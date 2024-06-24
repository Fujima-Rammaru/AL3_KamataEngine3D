#pragma once
#include "MatrixFunction.h"
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"
class Enemy {

public:

	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position);

	void Update();
	void Draw();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t txHandle_ = 0u;
	ViewProjection* viewProjection_ = nullptr;
	static inline const float kWalkSpeed = 0.05f;
	Vector3 velocity_ = {};
	static inline const float kWalkMotionAngleStart = 1.0f; 
	static inline const float kWalkMotionAngleEnd = 20.0f;
	static inline const float kWalkMotionTime = 1.0f;
	float walkTimer_ = 0.0f;
};