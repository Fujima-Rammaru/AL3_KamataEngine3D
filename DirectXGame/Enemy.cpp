#include "Enemy.h"
#include <algorithm>
#include <numbers>
void Enemy::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;

	txHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 0.5f;
	velocity_ = {-kWalkSpeed, 0, 0};
	walkTimer_ = 0.0f;
}

void Enemy::Update() {
	walkTimer_ += 1.0f / 60.0f;
	float param= std::sin(std::numbers::pi_v<float> * 2.0f*walkTimer_/kWalkMotionTime);
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param) ;
	worldTransform_.rotation_.x = std::numbers::pi_v<float> / 180.0f * radian;
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() { model_->Draw(worldTransform_, *viewProjection_, txHandle_); }
