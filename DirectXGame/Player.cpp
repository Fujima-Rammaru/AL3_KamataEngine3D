#include "Player.h"

void Player::initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {

	assert(model);
	model_ = model;
	txHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
}

Player::Player() {}

Player::~Player() {}

void Player::Update() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			worldTransform_.matWorld_.m[i][j] = 0;
		}	
	}
	worldTransform_.TransferMatrix(); }

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_, txHandle_); }
