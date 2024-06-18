#define NOMINMAX
#include "Player.h"
#include "Imgui.h"
#include "Input.h"
#include "MapChipField.h"
#include <algorithm>
#include <numbers>

void Player::initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position) {

	assert(model);
	model_ = model;

	txHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 5.0f / 2.0f; // 450度
}

Player::Player() {}

Player::~Player() {}

void Player::Update() {

	Move(); // 移動入力

	// 衝突情報を初期化
	CollisionMapInfo info;
	// 移動量に速度の値をコピー
	// info.move = velocity_;
	info.move = velocity_;
	// マップ衝突チェック
	CollisionMapCheckDown(info); // 下方向
	CollisionMapCheckUp(info);   // 天井のみ
	CollisionMapCheckLeft(info);
	CollisionMapCheckRight(info);

	// 判定結果を反映して移動する
	MoveByCollisionResult(info);

	// 天井に接触している場合の処理
	CollisionCeilingCase(info);

	// 壁に接触している場合の処理
	CollisionHitWallCase(info);
	// 接地状態の切り替え
	//	info.landing = landing;
	//  地面との当たり判定
	if (velocity_.y < 0) {
		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= kGroundPos) {
			landing = true;
		}
	}

	GroundStateChange(info);

	// velocity_ = info.move;

	// 移動
	worldTransform_.translation_.x += velocity_.x;

	velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed); // 最大速度制限

	// 旋回制御
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 0.0166f; // 1/60秒

		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float> * 5.0f / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

		// 状況に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		float timeRatio = 1 - turnTimer_ / kTimeTurn;
		float easing = EaseInOut(timeRatio);
		float nowRotationY = std::lerp(turnFirstRotationY_, destinationRotationY, easing); // 現在の角度
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = nowRotationY;
	}
	// Vector3 CR;
	// CR = CornerPosition(worldTransform_.translation_, kRightTop);
	//  行列計算
	ImGui::Text(
	    "onground=%d\ninfo.landing=%d\nvelocity.y=%3.2f\nlanding=%d\ninfo.hitWall=%d\ntransY=%3.2f", onGround_, info.landing, velocity_.y, landing, info.hitWall, worldTransform_.translation_.y);
	worldTransform_.UpdateMatrix();
}

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_, txHandle_); }

float Player::EaseInOut(float y) { return -(std::cosf(std::numbers::pi_v<float> * y) - 1 / 2); }

WorldTransform* Player::GetWorldTransform() { return &worldTransform_; }

void Player::Move() { // 移動入力
	if (onGround_) {  // 接地状態
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			// 左右加速
			Vector3 acceleration(0, 0, 0);
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}
			// 加速/減速
			velocity_.x += acceleration.x;
		} else {
			velocity_.x *= (1.0f - kAttenuation); // 非入力時は移動減衰する
		}
		// ジャンプ処理
		if (Input::GetInstance()->TriggerKey(DIK_UP)) { // 上キーを押した瞬間だけtrue
			landing = false;
			// ジャンプ初速
			velocity_.y += kJumpAcceleration;
		}
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に移行
			onGround_ = false;
		}
		// 空中
	} else {
		velocity_.y += -kGravityAcceleration;                  // 落下速度
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed); // 落下速度制限

		worldTransform_.translation_.y += velocity_.y;
		// 着地
		if (landing) {
			worldTransform_.translation_.y = kGroundPos;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
		}
	}
}

void Player::CollisionMapCheckUp(CollisionMapInfo& info) {
	if (info.move.y <= 0) {
		return;
	}
	// 移動後の4つの角座標
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	bool hit = false;

	// 左上点の判定
	IndexSet indexSet;

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {

		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
		//  めり込み先ブロックの範囲矩形
		BlockRect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		info.move.y = std::max(0.0f, (rect.bottom - worldTransform_.translation_.y) - (1.0f + kBlank)); //\\
		// 天井に当たったことを記録する
		info.ceiling = true;
	}
}

void Player::CollisionMapCheckDown(CollisionMapInfo& info) {
	if (info.move.y >= 0) {
		return;
	}

	// 移動後の4つの角座標
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	bool hit = false;

	// 左下点の判定
	IndexSet indexSet;
	Vector3 posNplusB;
	posNplusB = positionsNew[kLeftBottom] + vectorBlank;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(posNplusB);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定
	posNplusB = positionsNew[kRightBottom] + vectorBlank;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(posNplusB);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) { // ブロックにヒット?

		posNplusB = positionsNew[kLeftBottom] + vectorBlank;
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(posNplusB);

		//  めり込み先ブロックの範囲矩形
		BlockRect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		// float radius = (positionsNew[kRightTop].y - positionsNew[kRightBottom].y) / 2;
		info.move.y = std::min(0.0f, (rect.top - worldTransform_.translation_.y) + (1.0f + kBlank)); //\\
		// 地面に当たったことを記録する
		ImGui::Text("hit=%d", hit);
		info.landing = true;
	}
}

void Player::CollisionMapCheckLeft(CollisionMapInfo& info) {
	if (info.move.x >= 0) {
		return;
	}
	// 移動後の4つの角座標
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	bool hit = false;
	// 左上点の判定
	IndexSet indexSet;
	Vector3 posNplusB;
	posNplusB = positionsNew[kLeftTop] + Vector3(-0.2f, 0, 0);
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 左下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
		//  めり込み先ブロックの範囲矩形
		BlockRect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		info.move.x = std::min(0.0f, (rect.right - worldTransform_.translation_.x) + (1.0f + kBlank)); //\\
		// 壁に当たったことを記録する
		info.hitWall = true;
	}
}

void Player::CollisionMapCheckRight(CollisionMapInfo& info) {
	if (info.move.x <= 0) {
		return;
	}
	// 移動後の4つの角座標
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	bool hit = false;
	// 左上点の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 左下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
		//  めり込み先ブロックの範囲矩形
		BlockRect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		// float radius = (positionsNew[kRightBottom].x - positionsNew[kLeftBottom].x) / 2;
		info.move.x = std::max(0.0f, (rect.left - worldTransform_.translation_.x) - (1.0f + kBlank)); //\\
		// 壁に当たったことを記録する
		info.hitWall = true;
	}
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offSetTable[kNumCorner] = {
	    {kWidth / 2.0f,  -kHeight / 2.0f, 0}, //  kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kLeftBottom
	    {kWidth / 2.0f,  kHeight / 2.0f,  0}, //  kRightTop
	    {-kWidth / 2.0f, kHeight / 2.0f,  0}, //  kLeftTop
	};
	Vector3 result;

	result.x = center.x + offSetTable[static_cast<uint32_t>(corner)].x;
	result.y = center.y + offSetTable[static_cast<uint32_t>(corner)].y;
	result.z = center.z + offSetTable[static_cast<uint32_t>(corner)].z;
	return result;
}

void Player::MoveByCollisionResult(const CollisionMapInfo& info) {
	worldTransform_.translation_.x += info.move.x;
	worldTransform_.translation_.y += info.move.y;
	worldTransform_.translation_.z += info.move.z;
}

void Player::CollisionCeilingCase(const CollisionMapInfo& info) {
	// 天井に当たった?
	if (info.ceiling) {
		velocity_.y = 0;
	}
}

void Player::CollisionHitWallCase(const CollisionMapInfo& info) {
	if (info.hitWall) {
		velocity_.x *= (1.0f - kAttenuation);
	}
}

// 接地状態の切り替え処理
void Player::GroundStateChange(const CollisionMapInfo& info) {

	// 自キャラが接地状態?
	if (onGround_) {
		// 接地状態の処理
		if (velocity_.y > 0.0f) {
			onGround_ = false;

		} else {
			///==================================================================
			// 落下判定
			MapChipType mapChipType;

			// 移動後の4つの角座標
			std::array<Vector3, kNumCorner> positionsNew;
			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
			}

			// 真下の当たり判定を行う
			bool hit = false;
			// 左下点の判定
			IndexSet indexSet;
			Vector3 posNplusB;
			posNplusB = positionsNew[kLeftBottom] + vectorBlank;
			// positionsNew[kLeftBottom].y +=vectorBlank.y;
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(posNplusB);
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			// 右下点の判定
			posNplusB = positionsNew[kRightBottom] + vectorBlank;
			// positionsNew[kRightBottom].y +=vectorBlank.y;
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(posNplusB);
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}
			///==================================================================

			// 落下なら空中状態に切り替え
			if (!hit) {
				onGround_ = false;
			}
			//	ImGui::Text("hit=%d\nPosNewLeftB.y=%3.2f\nPosNewRightT.y=%3.2f\n", hit, positionsNew[kLeftBottom].y, positionsNew[kRightTop].y);
		}
	} else {
		if (info.landing) {
			onGround_ = true;
			velocity_.x *= (1.0f - kAttenuationLanding);
			velocity_.y = 0.0f;
			// 着地状態に切り替える
		}
	}
}

//
// void Player::cornerDetect(const MapChipType& mapChipType,const int& corner) {
//	IndexSet indexSet;
//	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);//指定した角の座標からマップチップ番号を取得
//	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
//	if (mapChipType == MapChipType::kBlock) {
//		hit = true;//ブロックが当たったとき
//	}
// }
