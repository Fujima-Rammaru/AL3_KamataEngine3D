#include "lightPowerItem.h"

void LightPowItem::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;

	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = 0.0f;
	worldTransform_.scale_ = Vector3(0.5f, 0.5f, 0.5f);
}

void LightPowItem::Update() {
	if (!isGetting_) {
		worldTransform_.UpdateMatrix();
	}
}

void LightPowItem::Draw() {
	if (isGetting_ == false) {
		model_->Draw(worldTransform_, *viewProjection_);
	}
}

AABB LightPowItem::GetAABB() {
	AABB aabb;

	Vector3 worldPos = GetWorldPosition();

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight, worldPos.z + kWidth / 2.0f};

	return aabb;
}

Vector3 LightPowItem::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

bool LightPowItem::IsFinishedGetter() { return isGetting_; }

void LightPowItem::Oncollision(const Player* player) {
	(void)player;
	isGetting_ = true;
}
