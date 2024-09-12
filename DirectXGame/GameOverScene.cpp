#include "GameOverScene.h"
#include "DirectXCommon.h"
#include "Input.h"

GameOverScene::~GameOverScene() {}

void GameOverScene::Initialize() {
	modelGameOver_ = Model::CreateFromOBJ("GameOver", true);
	viewProjection_.Initialize();
	worldTransform.Initialize();
	worldTransform.translation_ = {-11, -10, 0};
	worldTransform.rotation_ = {0, 3.24f, 0};
	worldTransform.scale_ = {5, 5, 5};
}

void GameOverScene::Update() {

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		IsFinished_ = true;
	}
	worldTransform.UpdateMatrix();
}

void GameOverScene::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);

	modelGameOver_->Draw(worldTransform, viewProjection_);

	Model::PostDraw();
}
