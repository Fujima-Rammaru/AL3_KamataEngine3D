#include "2Dspr.h"
#include "Vector4.h"

Box::Box() {}

Box::~Box() {}

void Box::Initialize() {
	textuteHandle = TextureManager::Load("sample.png");
	sprite_ = Sprite::Create(textuteHandle, position, color, anchorpoint, false, false);
	sprite_->SetSize(Vector2(100.0f, 100.0f));
	sprite_->SetColor(Vector4(1, 1, 1, 1));
}

void Box::Update() {  }

void Box::Draw(ID3D12GraphicsCommandList* commandList) {
	// DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	//// コマンドリストの取得
	// ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	Sprite::PreDraw(commandList);
	sprite_->Draw();
	Sprite::PostDraw();
}
