#pragma once
#include "MatrixFunction.h"
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

class MapChipField;

class Player {

private:
	struct CollisionMapInfo {
		bool ceiling = false; // 天井衝突フラグ
		bool landing = false;
		bool hitWall = false;
		Vector3 move = {};
	};

	enum Corner {
		kRightBottom,
		kLeftBottom,
		kRightTop,
		kLeftTop,
		kNumCorner // 要素数
	};

public:
	void initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position);
	Player();
	~Player();
	void Update();
	void Draw();
	float EaseInOut(float y);
	WorldTransform* GetWorldTransform();
	const Vector3& GetVelocity() const { return velocity_; };                          // 自キャラの速度を取得するためのgetter
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; } // setter
	void Move();
//	void CollisionMapCheck(CollisionMapInfo& info); // マップ衝突判定
	void CollisionMapCheckUp(CollisionMapInfo& info);
//	void CollisionMapCheckDown(CollisionMapInfo& info);
//	void CollisionMapCheckLeft(CollisionMapInfo& info);
//	void CollisionMapCheckRight(CollisionMapInfo& info);
	Vector3 CornerPosition(const Vector3& center, Corner corner);
	void MoveByCollisionMap(const CollisionMapInfo& info);
	void CollisionCeilingCase(const CollisionMapInfo& info);//天井に接触している場合の処理

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t txHandle_ = 0u;
	ViewProjection* viewProjection_ = nullptr;

	Vector3 velocity_ = {0, 0, 0};

	MapChipField* mapChipField_ = nullptr;

	static inline const float kAcceleration = 0.1f;
	static inline const float kAttenuation = 0.1f; // 速度減衰率
	static inline const float kLimitRunSpeed = 1.0f; // 最大速度制限
	static inline const float kBlank = 1.0f;

	enum class LRDirection {
		kRight,
		kLeft,
	};
	LRDirection lrDirection_ = LRDirection::kRight; // 右向きで初期化

	float turnFirstRotationY_ = 0.0f;                      // 旋回開始時の角度
	float turnTimer_ = 0.0f;                               // 旋回タイマー
	static inline const float kTimeTurn = 0.3f;            // 旋回時間<秒>
	bool onGround_ = true;                                 // 設置状態フラグ
	static inline const float kGravityAcceleration = 0.1f; // 重力加速度(下方向)
	static inline const float kLimitFallSpeed = 2.0f;      // 最大落下速度(下方向)
	static inline const float kJumpAcceleration = 2.0f;    // ジャンプ初速(上方向）
	static inline const float kGroundPos = 2.0f;           // 地面の座標
	bool landing = false;
	static inline const float kWidth = 1.8f; // キャラクターの当たり判定サイズ
	static inline const float kHeight = 1.8f;
};