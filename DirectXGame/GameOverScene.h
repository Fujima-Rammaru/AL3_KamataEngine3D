#pragma once
#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"
/// <summary>
/// タイトルシーン
/// </summary>

//前方宣言
class Player;

class GameOverScene {

public:
	~GameOverScene();
	void Initialize();
	void Update();
	void Draw();
	bool IsFinished() const { return IsFinished_; }

private:
	// 終了フラグ
	bool IsFinished_ = false;
	ViewProjection viewProjection_;
	WorldTransform worldTransform;
	Model* modelGameOver_ = nullptr;
};