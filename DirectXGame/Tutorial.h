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
	void Draw();
	bool IsFinished() const { return IsFinished_; }

private:

	// 終了フラグ
	bool IsFinished_ = false;
	// スプライト用変数
	Vector4 color = {1, 1, 1, 1};
	Vector2 position_ = {0.f, 0.f};
	Vector2 anchorpoint = {0, 0};
	Vector2 size_ = {1280.0f, 720.0f};
	Sprite* sprite_ = nullptr;
	uint32_t textuteHandle = 0u;
};
