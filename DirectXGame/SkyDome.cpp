#include "SkyDome.h"
#include "DirectXCommon.h"

void SkyDome::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);
	model_ = model;

	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
}

void SkyDome::Update() { worldTransform_.TransferMatrix(); }

void SkyDome::Draw() {

	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);
	model_->Draw(worldTransform_, *viewProjection_);

	Model::PostDraw();
}
