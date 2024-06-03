#pragma once
#include "MatrixFunction.h"
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

class Player {

public:
	void initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position);
	Player();
	~Player();
	void Update();
	void Draw();
	float EaseInOut(float y);
	WorldTransform* GetWorldTransform();
	const Vector3& GetVelocity() const { return velocity_; };//自キャラの速度を取得するためのgetter

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t txHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

	Vector3 velocity_ = {0, 0, 0};

	static inline const float kAcceleration = 0.1f;
	static inline const float kAttenuation = 0.1f; // 速度減衰率

	static inline const float kLimitRunSpeed = 1.0f; // 最大速度制限

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
	static inline const float kLimitFallSpeed = 1.0f;      // 最大落下速度(下方向)
	static inline const float kJumpAcceleration = 2.0f;    // ジャンプ初速(上方向）
	static inline const float kGroundPos = 2.0f;           // 地面の座標
	bool landing = false;
	
};