#define NOMINMAX
#include "Player.h"
#include "ImGuiManager.h"
#include "Input.h"
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

	// 移動入力
	if (onGround_) { // 接地状態
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			// 左右加速
			Vector3 acceleration = {};
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
	// 地面との当たり判定
	if (velocity_.y < 0) {
		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= kGroundPos) {
			landing = true;
		}
	}

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

	// 移動
	worldTransform_.translation_.x += velocity_.x;

	// 行列計算
	worldTransform_.UpdateMatrix();
#ifdef _DEBUG
	ImGui::Text(
	    "rotation.y=%5.2f\nvelocity.y=%5.2f\nvelocity.y=%5.2f\nonGroud=%d\nlanding=%d", worldTransform_.rotation_.y, velocity_.y,velocity_.y, onGround_,
	    landing); // デバッグテキスト
#endif
}

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_, txHandle_); }

float Player::EaseInOut(float y) { return -(std::cosf(std::numbers::pi_v<float> * y) - 1 / 2); }
