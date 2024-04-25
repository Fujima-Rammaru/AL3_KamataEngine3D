#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

class Player {

public:
	void initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection);

	Player();

	~Player();

	void Update();

	void Draw();

private:
	
	WorldTransform worldTransform_;

	Model* model_ = nullptr;


	uint32_t txHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

	ObjectColor* objectColor_ = nullptr;
};