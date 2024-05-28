
#pragma once
class Player;
#include "ViewProjection.h"
#include "WorldTransform.h"

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

	

private:
	ViewProjection viewProjection_;
	Player* target_ = nullptr;                    // 追従対象のポインタ
	const Vector3 targetOffset_ = {0, 0, -15.0f}; // 追従対象とカメラの座標の差
	WorldTransform worldTransform_;
};