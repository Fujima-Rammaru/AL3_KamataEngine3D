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
	delete modelPlayer_;
	delete block_;
	delete player_;
	delete debugCamera_;
	delete skyDome_;
	delete modelSkyDome_;
	delete mapChipField_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	txHandle_ = TextureManager::Load("sample.png"); // テクスチャの読み込み
	// uint32_t skyDomeTxHandle_ = TextureManager::Load("uvChecker.png"); // テクスチャの読み込み
	blockTxHandle_ = TextureManager::Load("cube/cube.jpg");
	modelPlayer_ = Model::Create(); // 3Dモデルの生成
	block_ = Model::Create();       // 3Dモデルの生成

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	player_ = new Player(); // 自キャラの生成

	skyDome_ = new SkyDome();                              // 天球の生成
	modelSkyDome_ = Model::CreateFromOBJ("SkyDome", true); // 3Dモデルの生成
	skyDome_->Initialize(modelSkyDome_, &viewProjection_); // 天球の初期化
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	debugCamera_->SetFarZ(5000);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/MapChip.csv"); // CSVファイル読み込み
	GenerateBlocks();

	Vector3 playerposition = mapChipField_->GetMapChipPositionByIndex(2, 15);
	player_->initialize(modelPlayer_, txHandle_, &viewProjection_, playerposition); // 自キャラの初期化
}

void GameScene::Update() {
	Vector3 scale{1.0f, 1.0f, 1.0f};     // 拡縮
	Vector3 rotate{1.0f, 1.0f, 1.0f};    // 回転
	Vector3 translate{1.0f, 1.0f, 1.0f}; // 平行移動

	MatrixFunction* matrixFunction;
	matrixFunction = new MatrixFunction;

	for (auto& worldTransformBlockLine : worldTransformBlocks_) {
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
	skyDome_->Update();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_BACK)) {
		isDebugCameraactive_ ^= true;
	}
	if (isDebugCameraactive_) {
		debugCamera_->Update();
		//	viewProjection_.matView = debugCamera_; /////
		// viewProjection_.matProjection=

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
	// modelPlayer_->Draw(worldTransform_, debugCamera_->GetViewProjection());
	modelSkyDome_->Draw(worldTransform_, debugCamera_->GetViewProjection());
	player_->Draw();
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

void GameScene::GenerateBlocks() {
	uint32_t kNumBlockHorizontal = mapChipField_->GetNumBlockHorizontal(); 
	uint32_t kNumBlockVirtical = mapChipField_->GetNumBlockVirtical();     
	                                                                      	                                                                      
	// 要素数を変更する
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t y = 0; y < kNumBlockVirtical; y++) { // キューブの生成
		worldTransformBlocks_[y].resize(kNumBlockHorizontal);

		for (uint32_t x = 0; x < kNumBlockHorizontal; ++x) {
			worldTransformBlocks_[y][x] = new WorldTransform();
			WorldTransform* worldTransform = new WorldTransform();

			if (mapChipField_->GetMapChipTypeByIndex(x, y) == MapChipType::kBlock) {

				worldTransformBlocks_[y][x] = worldTransform;
				// worldTransformBlocks_[y][x]->Initialize();
				worldTransformBlocks_[y][x]->translation_ = mapChipField_->GetMapChipPositionByIndex(x, y);
			}
			worldTransformBlocks_[y][x]->Initialize();
		}
	}
}
