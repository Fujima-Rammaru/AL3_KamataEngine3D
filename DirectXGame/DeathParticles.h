#pragma once
#include "MatrixFunction.h"
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"
#include <array>
#include <numbers>
/// <summary>
/// デス演出用パーティクル
/// </summary>
class DeathParticles {

public:
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

private:
	Model* model_ = nullptr;
	uint32_t txHandle_ = 0u;
	ViewProjection* viewProjection_ = nullptr;
	static inline const uint32_t kNumParticles = 8;//パーティクルの個数
	std::array<WorldTransform, kNumParticles> worldTransforms_;
	static inline const float kDuration = 2.0f;
	static inline const float kSpeed = 0.2f;
	static inline const float kAngleUnit = std::numbers::pi_v<float> * 2.0f / 8.0f;
};