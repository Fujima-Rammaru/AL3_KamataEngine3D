#include "GameScene.h"
#include "DebugCamera.h"
#include "MatrixFunction.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	delete model_;
	delete block_;
	delete player_;
	delete debugCamera_;
	delete skyDome_;
	delete modelSkyDome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	txHandle_ = TextureManager::Load("sample.png"); // テクスチャの読み込み
	blockTxHandle_ = TextureManager::Load("cube/cube.jpg");
	model_ = Model::Create(); // 3Dモデルの生成
	block_ = Model::Create(); // 3Dモデルの生成
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	player_ = new Player();                                   // 自キャラの生成
	player_->initialize(model_, txHandle_, &viewProjection_); // 自キャラの初期化

	skyDome_ = new SkyDome();                              // 天球の生成
	skyDome_->Initialize();                                // 天球の初期化
	modelSkyDome_ = Model::CreateFromOBJ("SkyDome", true); // 3Dモデルの生成

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	const uint32_t kNumBlockHorizontal = 20; // 要素数
	const uint32_t kNumBlockVirtical = 10;   // 要素数
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;
	// 要素数を変更する

	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t y = 0; y < kNumBlockVirtical; y++) { // キューブの生成
		worldTransformBlocks_[y].resize(kNumBlockHorizontal);
		for (uint32_t x = 0; x < kNumBlockHorizontal; ++x) {
			worldTransformBlocks_[y][x] = new WorldTransform();
			worldTransformBlocks_[y][x]->Initialize();

			if (x % 2 == 0 && y % 2 == 0) {
				worldTransformBlocks_[y][x]->translation_.x = kBlockWidth * x;
				worldTransformBlocks_[y][x]->translation_.y = kBlockHeight * y;
				/*worldTransformBlocks_[y][x]->translation_.x = NULL;
				worldTransformBlocks_[y][x]->translation_.y = NULL;*/
			} else if (x % 2 == 1 && y % 2 == 1) {
				/*worldTransformBlocks_[y][x]->translation_.x = NULL;
				worldTransformBlocks_[y][x]->translation_.y = NULL;*/
				worldTransformBlocks_[y][x]->translation_.x = kBlockWidth * x;
				worldTransformBlocks_[y][x]->translation_.y = kBlockHeight * y;
			}
		}
	}
}

void GameScene::Update() {
	Vector3 scale{1.0f, 1.0f, 1.0f};     // 拡縮
	Vector3 rotate{1.0f, 1.0f, 1.0f};    // 回転
	Vector3 translate{1.0f, 1.0f, 1.0f}; // 平行移動

	MatrixFunction* matrixFunction;
	matrixFunction = new MatrixFunction;
	for (const auto& worldTransformBlockLine : worldTransformBlocks_) {
		for (auto worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}

			worldTransformBlock->matWorld_ = matrixFunction->MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			// 定数バッファに転送する
			worldTransformBlock->TransferMatrix();
		}
	}

	player_->Update();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_BACK)) {
		isDebugCameraactive_ ^= true;
	}

	if (isDebugCameraactive_) {
		debugCamera_->Update();

		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}
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
	// player_->Draw();

	for (const auto& worldTransformBlockLine : worldTransformBlocks_) {
		for (auto worldTransformBlock : worldTransformBlockLine) {
			block_->Draw(*worldTransformBlock, debugCamera_->GetViewProjection());
		}
	}
	// model_->Draw(worldTransform_, viewProjection);
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
