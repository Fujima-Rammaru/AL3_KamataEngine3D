
#pragma once
class Player;
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "stdlib.h"
#include "time.h"
// 矩形
struct Rect {
	float left = 0.0f;
	float right = 1.0f;
	float bottom = 0.0f;
	float top = 1.0f;
};

/// <summary>
/// カメラコントローラー
/// </summary>
class CameraController {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void SetTarget(Player* target) { target_ = target; };

	void Reset();

	void SetMovableArea(Rect area) { movableArea_ = area; }

	Matrix4x4 GetMatView();

	Matrix4x4 GetMatProjection();

	Vector3 LerpVector3();

	void CameraShake();

	void SetTargetOffset(const Vector3& targetOffset);

private:
	ViewProjection viewProjection_;
	Player* target_ = nullptr;              // 追従対象のポインタ
	Vector3 targetOffset_ = {0, 0, -30.0f}; // 追従対象とカメラの座標の差
	WorldTransform worldTransform_;
	Rect movableArea_ = {0, 0, 0, 0};                                 // カメラ移動範囲
	Vector3 goalPosition;                                             // カメラの目標座標
	static inline const float kInterpolationRate = 0.2f;              // 座標補間割合
	static inline const float kVelocityBiasX = 10.0f;                 // 速度掛け率X
	static inline const float kVelocityBiasY = 0.0f;                  // 速度掛け率Y
	static inline const Rect kTargetMovableArea = {-10, 10, -10, 10}; // 追従対象の各方向へのカメラ移動範囲
	Vector3 targetVelocity = {0, 0, 0};                               // 追従対象の加速度を受け取るためのメンバ変数
	unsigned int currentTime = int(time(nullptr));
	Vector3 shakeRange = {0, 0, 0}; // カメラの揺れ幅
	int shakeTimer = 120;           // シェイクの時間
	float offSet = 0;
	bool isShaking = false;
	bool isCountingTimer = false;
	Vector3 shakePosition = {0, 0, 0};
};