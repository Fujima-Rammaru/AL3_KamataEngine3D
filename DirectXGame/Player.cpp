#include "Player.h"

void Player::initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {

	assert(model);
	model_ = model;
	txHandle_ = textureHandle;
	worldTrasform_.Initialize();
	viewProjection_ = viewProjection;
}

void Player::Update() { worldTrasform_.TransferMatrix(); }

void Player::Draw() {
	model_->Draw(worldTrasform_,viewProjection_,txHandle_); 
}
