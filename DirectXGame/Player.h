#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"

class Player {

public:
	void initialize(Model* model,uint32_t textureHandle,ViewProjection* viewProjection);

	void Update();

	void Draw();

private:
	// ワールド変換データ
	WorldTransform worldTrasform_;

	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t txHandle_ = 0u;

	ViewProjection* viewProjection_=nullptr;
};