#pragma once
#include "DirectXCommon.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "Vector2.h"

class Tutorial {
public:
	// コマンドリストの取得
	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList);
	Vector2 GetPosition();
	Vector2 GetSize();
	void Setposition(const Vector2& position);
	void SetSize(const Vector2& size);


private:


};

