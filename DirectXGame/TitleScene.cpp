#include "TitleScene.h"
#include "Input.h"

void TitleScene::Initialize() { 
	
}

void TitleScene::Update() {

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		IsFinished_ = true;
	}
}

void TitleScene::Draw() { }
