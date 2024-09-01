#include "TitleScene.h"
#include "DirectXCommon.h"
#include "Input.h"

TitleScene::~TitleScene() { delete modelTitle_; }

void TitleScene::Initialize() {
	modelTitle_ = Model::CreateFromOBJ("title", true);
	viewProjection_.Initialize();
	worldTransform.Initialize();
	worldTransform.translation_ = {0, 10, 0};
	worldTransform.rotation_ = {-4.4f,3.2f,0};
	worldTransform.scale_ = {10, 10, 10};
}

void TitleScene::Update() {

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		IsFinished_ = true;
	}
	//viewProjection_.UpdateMatrix();
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
