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
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();
	bool IsFinished() const { return isFinished_; };

private:
	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;
	static inline const uint32_t kNumParticles = 8; // パーティクルの個数
	std::array<WorldTransform, kNumParticles> worldTransforms_;
	static inline const float kDuration = 1.0f; // 存続期間(消滅までの時間)<秒>
	static inline const float kSpeed = 0.2f;
	static inline const float kAngleUnit = std::numbers::pi_v<float> * 2.0f / 8.0f; // 分割した1個分の角度
	bool isFinished_ = false;
	float counter_ = 0.0f;
	ObjectColor objectColor_; // 色変更オブジェクト
	Vector4 color_;           // 色の数値
	float arphaVal = 2.0f;
};