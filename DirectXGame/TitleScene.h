#pragma once


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
	//終了フラグ
	bool IsFinished_ = false;
};