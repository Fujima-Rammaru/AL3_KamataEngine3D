#include "2Dspr.h"
#include "Vector4.h"

Box::Box() {}

Box::~Box() {}

void Box::Initialize() {
	textuteHandle = TextureManager::Load("light.png");
	sprite_ = Sprite::Create(textuteHandle, position_, color, anchorpoint, false, false);
	sprite_->SetSize(size_);
	sprite_->SetColor(Vector4(1, 1, 1, 1));
}

void Box::Update() {
	sprite_->SetPosition(position_);
	sprite_->SetSize(size_);
	
}

void Box::Draw(ID3D12GraphicsCommandList* commandList) {
	// DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	//// コマンドリストの取得
	// ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	Sprite::PreDraw(commandList);
	sprite_->Draw();
	Sprite::PostDraw();
}

// Getter
Vector2 Box::GetPosition() { return position_; }
Vector2 Box::GetSize() { return size_; }

// Setter
void Box::Setposition(const Vector2& position) { position_ = position; }
void Box::SetSize(const Vector2& size) { size_ = size; }
