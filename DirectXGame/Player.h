#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"
#include"MatrixFunction.h"

class Player {

public:
	void initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position);

	Player();

	~Player();

	void Update();

	void Draw();

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t txHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

	Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.3f;
};