#include "ClearScene.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "TitleScene.h"

ClearScene::~ClearScene() { delete modelClear_; }

void ClearScene::Initialize() {
	modelClear_ = Model::CreateFromOBJ("Clear", true);
	viewProjection_.Initialize();
	worldTransform.Initialize();
	worldTransform.translation_ = {0, 0, 0};
	worldTransform.rotation_ = {-4.8f, 3.2f, 0};
	worldTransform.scale_ = {8, 8, 8};
}

void ClearScene::Update() {

	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		IsFinished_ = true;
	}
	// viewProjection_.UpdateMatrix();
	worldTransform.UpdateMatrix();
}

void ClearScene::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);

	modelClear_->Draw(worldTransform, viewProjection_);

	Model::PostDraw();
}