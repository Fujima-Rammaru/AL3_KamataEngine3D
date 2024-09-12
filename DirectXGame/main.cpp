#include "Audio.h"
#include "AxisIndicator.h"
#include "ClearScene.h"
#include "DirectXCommon.h"
#include "GameOverScene.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "TitleScene.h"
#include "WinApp.h"

GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
ClearScene* clearScene = nullptr;
GameOverScene* gameOverScene = nullptr;

enum class Scene {
	kUnknown = 0,
	kTitle,
	kGame,
	kClear,
	kGameOver,
};

// 現在シーン（型）
Scene scene = Scene::kUnknown;

void ChangeScene();

void UpdateScene();

void DrawScene();

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();

	win->CreateGameWindow(L"GC2A_10_フジマ_ランマル_AL3");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();

	// 最初のシーンの初期化
	scene = Scene::kTitle;
	titleScene = new TitleScene();
	titleScene->Initialize();

#ifdef _DEBUG
	scene = Scene::kGame;
	gameScene = new GameScene();
	gameScene->Initialize();

#endif // _DEBUG

#pragma endregion

	//// ゲームシーンの初期化

	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();
		// シーン切り替え
		ChangeScene();
		// 現在シーンの更新
		UpdateScene();
		// 軸表示の更新
		axisIndicator->Update();
		// ImGui受付終了
		imguiManager->End();
		// 描画開始
		dxCommon->PreDraw();
		// 現在シーンの描画
		DrawScene();
		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	delete titleScene;
	delete gameScene;
	delete gameOverScene;
	delete clearScene;

	// 3Dモデル解放
	Model::StaticFinalize();
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}

void ChangeScene() {

	switch (scene) {

	case Scene::kTitle: // タイトル

		if (titleScene->IsFinished()) {
			// シーン変更
			scene = Scene::kGame;
			// 旧シーンの開放
			delete titleScene;
			titleScene = nullptr;
			// 新シーンの生成と初期化
			gameScene = new GameScene();
			gameScene->Initialize();
		}
		break;

	case Scene::kGame: // プレイ画面

		/// ゲームオーバー時のシーン遷移処理==========================
		if (gameScene->IsFinished()) {
			// シーン変更
			scene = Scene::kGameOver;
			// 現在シーンの開放
			delete gameScene;
			gameScene = nullptr;
			// 次シーンの生成と初期化
			gameOverScene = new GameOverScene();
			gameOverScene->Initialize();
		}
		//// ゲームクリア時のシーン遷移処理
		//else if () {
		//	// シーン変更
		//	scene = Scene::kClear;
		//	// 現在シーンの開放
		//	delete gameScene;
		//	gameScene = nullptr;
		//	// 次シーンの生成と初期化
		//	clearScene = new ClearScene();
		//	clearScene->Initialize();
		//}
		break;

	case Scene::kGameOver: // ゲームオーバー

		if (gameOverScene->IsFinished()) {
			// シーン変更
			scene = Scene::kTitle;
			// 現在シーンの開放
			delete gameOverScene;
			gameOverScene = nullptr;
			// 次シーンの生成と初期化
			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;

	case Scene::kClear: // ゲームクリア

		if (clearScene->IsFinished()) {
			// シーン変更
			scene = Scene::kTitle;
			// 現在シーンの開放
			delete clearScene;
			clearScene = nullptr;
			// 次シーンの生成と初期化
			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;
	}
}

void UpdateScene() {
	switch (scene) {

	case Scene::kTitle:
		titleScene->Update();
		break;

	case Scene::kGame:
		gameScene->Update();
		break;

	case Scene::kGameOver:
		gameOverScene->Update();
		break;

	case Scene::kClear:
		clearScene->Update();
		break;
	}
}

void DrawScene() {
	switch (scene) {

	case Scene::kTitle:
		titleScene->Draw();
		break;

	case Scene::kGame:
		gameScene->Draw();
		break;

	case Scene::kGameOver:
		gameOverScene->Draw();
		break;

	case Scene::kClear:
		clearScene->Draw();
		break;
	}
}
