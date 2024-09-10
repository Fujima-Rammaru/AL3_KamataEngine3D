#include "Enemy.h"
#include "Player.h"
#include "mathFunction.h"
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

	/// ホーミング============================================
	t += 1.0f / 100.0f;
	if (t >= 0.05f) {
		t = 0.0f;
	}
	// 敵からplayerへのベクトルを計算
	Vector3 toPlayer = player_->GetWorldPosition();
	// ベクトルを正規化
	toPlayer = Normalize(Subtract(toPlayer, worldTransform_.translation_));
	velocity_ = Normalize(velocity_);
	// 線形補間
	velocity_ = Multiply(kWalkSpeed, Slerp(velocity_, toPlayer, t));
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
	///=======================================================
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
