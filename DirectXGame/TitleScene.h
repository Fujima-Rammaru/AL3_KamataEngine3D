#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene {

public:
	void Initialize();
	void Update();
	void Draw();
	bool IsFinished() const { return IsFinished_; }
	

private:
	// 終了フラグ
	bool IsFinished_ = false;
	
};