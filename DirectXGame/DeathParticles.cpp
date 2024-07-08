#include "DeathParticles.h"
#include "MatrixFunction.h"
#include <algorithm>
void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;
	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
	// ワールド変換の初期化
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
}

void DeathParticles::Update() {
	// 終了なら何もしない
	if (isFinished_) {
		arphaVal = 2.0f;
		return;
	}

	color_.w = std::clamp(arphaVal, 0.0f, 1.0f);
	// 色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);
	// 色変更オブジェクトをVRAMに転送
	objectColor_.TransferMatrix();
	arphaVal -= 2.0f / 60.0f;
	counter_ += 1.0f / 60.0f;
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		// 終了扱いにする
		isFinished_ = true;
	}
	for (uint32_t i = 0; i < 8; ++i) {
		Vector3 velocity = {kSpeed, 0, 0};

		float angle = kAngleUnit * i;

		Matrix4x4 matrixRotation = MatrixFunction::MakeRotateZMatrix(angle);

		velocity = MatrixFunction::Transform(velocity, matrixRotation);

		worldTransforms_[i].translation_ += velocity;
	}

	for (auto& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}
}

void DeathParticles::Draw() {

	if (isFinished_) {
		return;
	}
	for (auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *viewProjection_,&objectColor_);
	}
}


