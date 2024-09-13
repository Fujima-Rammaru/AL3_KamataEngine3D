#include "Tutorial.h"
#include "DirectXCommon.h"
#include "Input.h"

void Tutorial::Initialize() {
	textuteHandle = TextureManager::Load("Tutorial.png");
	sprite_ = Sprite::Create(textuteHandle, position_, color, anchorpoint, false, false);
	sprite_->SetSize(size_);
	sprite_->SetColor(Vector4(1, 1, 1, 1));
}

void Tutorial::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		IsFinished_ = true;
	}
}

void Tutorial::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	Sprite::PreDraw(commandList);
	sprite_->Draw();
	Sprite::PostDraw();
}
