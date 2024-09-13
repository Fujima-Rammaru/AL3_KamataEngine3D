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
	Vector2 GetPosition();
	Vector2 GetSize();
	void Setposition(const Vector2& position);
	void SetSize(const Vector2& size);

private:
	// スプライト用変数
	Vector4 color = {1, 1, 1, 1};
	Vector2 position_ = {-1273.0f, -720.0f};
	Vector2 anchorpoint = {0, 0};
	Vector2 size_ = {3840.0f, 2160.0f};
	Sprite* sprite_ = nullptr;
	uint32_t textuteHandle = 0u;
};
