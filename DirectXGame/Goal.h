#pragma once
#include "AABB.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

class Player;

class Goal {
public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

	AABB GetAABB();

	Vector3 GetWorldPosition();
	// bool型のゲッター
	bool IsFinishedGetter();
	// 衝突応答
	void Oncollision(const Player* player);

private:
	// 横幅
	static inline const float kWidth = 1.0f;
	// 縦幅
	static inline const float kHeight = 4.0f;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;
	bool isFinished_ = false;
};
