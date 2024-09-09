#include "Enemy.h"
#include <algorithm>
#include <numbers>
#include"mathFunction.h"
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
	/*float param = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / kWalkMotionTime);
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param);*/
	//worldTransform_.rotation_.x = std::numbers::pi_v<float> / 180.0f * radian;
	//worldTransform_.translation_ += velocity_;
	worldTransform_.rotation_.x += 0.1f;
	worldTransform_.UpdateMatrix();

	//homming

	//敵からplayerへのベクトルを計算
	Vector3 toPlayer =

	    // ベクトルを正規化
	    Normalize(toPlayer);
	    Normalize(velocity_);
	//線形補間
	    velocity_ = lerp()
	    //進行方向に見た目の回転を合わせる
}

void Enemy::Draw() { model_->Draw(worldTransform_, *viewProjection_, txHandle_); }

AABB Enemy::GetAABB() {
	AABB aabb;

	Vector3 worldPos = GetWorldPosition();

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight, worldPos.z + kWidth / 2.0f};

	return aabb;
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision(const Player* player) { (void)player; }
