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
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 1.0f;
	velocity_ = {-kWalkSpeed, 0, 0};
}

void Enemy::Update() {
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix(); }

void Enemy::Draw() { model_->Draw(worldTransform_, *viewProjection_, txHandle_); }
