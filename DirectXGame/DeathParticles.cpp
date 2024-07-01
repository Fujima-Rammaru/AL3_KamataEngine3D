#include "DeathParticles.h"
#include "MatrixFunction.h"

void DeathParticles::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;

	txHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	// ワールド変換の初期化
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
}

void DeathParticles::Update() {
	
	for (uint32_t i = 0; i < 8; ++i) {
		Vector3 velocity = {kSpeed, 0, 0};

		float angle = kAngleUnit * i;

		Matrix4x4 matrixRotation = MatrixFunction::MakeRotateZMatrix(angle);

		velocity=
	}
	
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}
}

void DeathParticles::Draw() {
	for (auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *viewProjection_, txHandle_);
	}
}
