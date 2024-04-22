#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete sprite_;
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	txHandle_ = TextureManager::Load("sample.png"); // テクスチャを読み込む
	sprite_ = Sprite::Create(txHandle_, {100, 50}); // スプライトの生成
	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	soundDataHandle_ = audio_->LoadWave("fanfare.wav");                  // サウンドデータの読みこみ
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);             // 音声再生
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_); // ライン描画が参照するビュープロジェクションを指定する(アドレス渡し）
	debugCamera_ = new DebugCamera(1280, 720);                           // デバッグカメラの作成
	// 軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {
	Vector2 position = sprite_->GetPosition();
	position.x += 2.0f;
	position.y += 1.0f;
	sprite_->SetPosition(position);
	debugCamera_->Update(); // デバッグカメラの更新

	if (input_->TriggerKey(DIK_SPACE)) {
		if (audio_->IsPlaying(voiceHandle_)) {
			// 音声停止
			audio_->StopWave(voiceHandle_);
		} else {
			voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);
		}
	}

#ifdef _DEBUG
	// デバッグモードでのみ使用可能
	ImGui::Begin("Debug1");
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	ImGui::SliderFloat3("sliderFloat3", inputFloat3, 0.0f, 1.0f);
	// ImGui::Text("Kamata Tarou %d.%d.%d", 2050, 12, 31); // デバッグテキストの表示
	ImGui::End();
	ImGui::ShowDemoWindow();
#endif
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), txHandle_);
	/// </summary>

	// 3Dオブジェクト描画後処理

	float lineWidth = 40;
	float lineHeight = 40;
	float lineSpanH = 2;
	float lineSpanV = 2;

	// ラインを描画する
	for (float h = -lineWidth / 2; h <= lineWidth / 2; h += lineSpanH) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		    {h, -lineHeight / 2, 0}, // w=15
		    {h, lineHeight / 2, 0},  // h=2
		    {1.0f, 0.0f, 0.0f, 1.0f});
	}
	for (float v = -lineHeight / 2; v <= lineHeight / 2; v += lineSpanV) {
		PrimitiveDrawer::GetInstance()->DrawLine3d({-lineWidth / 2, v, 0}, {lineWidth / 2, v, 0}, {0.0f, 0.0f, 1.0f, 1.0f});
	}

	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	sprite_->Draw();
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
