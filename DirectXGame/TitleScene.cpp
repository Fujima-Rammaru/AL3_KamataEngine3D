#include "TitleScene.h"
#include "DirectXCommon.h"
#include "Input.h"

TitleScene::~TitleScene() { delete modelTitle_; }

void TitleScene::Initialize() {
	modelTitle_ = Model::CreateFromOBJ("Title", true);
	viewProjection_.Initialize();
	worldTransform.Initialize();
	worldTransform.translation_ = {-11, -10, 0};
	worldTransform.rotation_ = {0, 3.24f, 0};
	worldTransform.scale_ = {5, 5, 5};
}

void TitleScene::Update() {

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		IsFinished_ = true;
	}
	worldTransform.UpdateMatrix();
}

void TitleScene::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);

	modelTitle_->Draw(worldTransform, viewProjection_);

	Model::PostDraw();
}
