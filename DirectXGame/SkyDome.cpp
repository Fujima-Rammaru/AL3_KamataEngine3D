#include "SkyDome.h"

void SkyDome::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);
	model_ = model;

	viewProjection_ = viewProjection;
	
	worldTransform_.Initialize();

}

void SkyDome::Update() { worldTransform_.TransferMatrix(); }

void SkyDome::Draw() { model_->Draw(worldTransform_, *viewProjection_); }
