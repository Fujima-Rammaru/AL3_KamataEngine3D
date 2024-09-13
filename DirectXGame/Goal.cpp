#include "Goal.h"

void Goal::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;

	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = -1.2f;
	worldTransform_.scale_ = Vector3(0.5f, 0.5f, 0.5f);
}

void Goal::Update() { worldTransform_.UpdateMatrix(); }

void Goal::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

AABB Goal::GetAABB() {
	AABB aabb;

	Vector3 worldPos = GetWorldPosition();

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight, worldPos.z + kWidth / 2.0f};

	return aabb;
}

Vector3 Goal::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

bool Goal::IsFinishedGetter() { return isFinished_; }

void Goal::Oncollision(const Player* player) {
	(void)player;
	isFinished_ = true;
}
