#pragma once
#include "DirectXCommon.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "Vector2.h"

class Box {
public:
	Box();
	~Box();
	// コマンドリストの取得
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList);

private:
	// スプライト用変数
	Vector4 color = {1, 1, 1, 1};
	Vector2 position = {0, 0};
	Vector2 anchorpoint = {0, 0};
	Vector2 size = {100.0f, 100.0f};
	Sprite* sprite_ = nullptr;
	uint32_t textuteHandle = 0u;

	
};
