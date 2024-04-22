#include "Player.h"

void Player::initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {

	assert(model);
	model_ = model;
	txHandle_ = textureHandle;
	worldTrasform_;
	viewProjection_ = viewProjection;
}

void Player::Update() { worldTrasform_.TransferMatrix(); }

void Player::Draw() {
	model_->Draw(Player::worldTrasform_,Player::viewProjection_,Player::txHandle_); 
}
