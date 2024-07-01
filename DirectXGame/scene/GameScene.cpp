#include "GameScene.h"
#include "DebugCamera.h"
#include "TextureManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() {

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	delete modelPlayer_;
	delete modelBlock_;
	delete modelEnemy;
	delete enemy_;
	delete player_;
	delete debugCamera_;
	delete skyDome_;
	delete modelSkyDome_;
	delete mapChipField_;
	delete cameraController_;
	delete modelParticles_;
	delete deathParticles_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	playerTxHandle_ = TextureManager::Load("sample.png"); // テクスチャの読み込み
	blockTxHandle_ = TextureManager::Load("cube/cube.jpg");
	modelPlayer_ = Model::Create(); // 3Dモデルの生成
	modelBlock_ = Model::Create();

	worldTransform_.Initialize();
	viewProjection_.Initialize();
	cameraViewProjection_.Initialize();

	skyDome_ = new SkyDome();                              // 天球の生成
	modelSkyDome_ = Model::CreateFromOBJ("SkyDome", true); // 3Dモデルの生成
	skyDome_->Initialize(modelSkyDome_, &viewProjection_); // 天球の初期化
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	debugCamera_->SetFarZ(5000);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/MapChip.csv"); // CSVファイル読み込み
	GenerateBlocks();

	player_ = new Player(); // 自キャラの生成
	Vector3 playerposition = mapChipField_->GetMapChipPositionByIndex(3, 18);
	player_->initialize(modelPlayer_, playerTxHandle_, &cameraViewProjection_, playerposition); // 自キャラの初期化
	player_->SetMapChipField(mapChipField_);
	deathParticles_ = new DeathParticles();
	modelParticles_ = Model::CreateFromOBJ("Particle", true);
	deathParticles_->Initialize(modelParticles_, &cameraViewProjection_, playerposition);

	// 敵キャラの生成
	enemyTxhandle = TextureManager::Load("sample.png"); // テクスチャの読み込み
	modelEnemy = Model::Create();
	enemy_ = new Enemy();
	Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(13, 18);
	enemy_->Initialize(modelEnemy, enemyTxhandle, &cameraViewProjection_, enemyPosition);

	// カメラコントローラー初期化
	Rect area_ = {30.0f, 170.0f, 0.0f, 100.0f};
	cameraController_ = new CameraController;
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();
	cameraController_->SetMovaAbleArea(area_);
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
	enemy_->Update();
	if (deathParticles_) {
		deathParticles_->Update();
	}

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_BACK)) {
		isDebugCameraactive_ ^= true;
	}
	if (isDebugCameraactive_) {
		cameraController_->Update();
		cameraViewProjection_.matView = cameraController_->GetMatView();

		cameraViewProjection_.matProjection = cameraController_->GetMatProjection();

		cameraViewProjection_.TransferMatrix();
	} else {
		cameraViewProjection_.UpdateMatrix();
	}

#endif
	CheckAllCollisions();
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

	for (const auto& worldTransformBlockLine : worldTransformBlocks_) {
		for (auto worldTransformBlock : worldTransformBlockLine) {
			modelBlock_->Draw(*worldTransformBlock, cameraViewProjection_);
		}
	}

	modelSkyDome_->Draw(worldTransform_, cameraViewProjection_);
	player_->Draw();
	enemy_->Draw();
	if (deathParticles_) {
		deathParticles_->Draw();
	}

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
				worldTransformBlocks_[y][x]->translation_ = mapChipField_->GetMapChipPositionByIndex(x, y);
			}
			worldTransformBlocks_[y][x]->Initialize();
		}
	}
}

void GameScene::CheckAllCollisions() {

#pragma region 自キャラと敵の当たり判定
	AABB aabb1, aabb2;
	// 自キャラの座標
	aabb1 = player_->GetAABB();
	// 敵の座標
	aabb2 = enemy_->GetAABB();

	// AABB同士の交差判定
	if (aabb1.isHit(aabb2)) {
		player_->OnCollision(enemy_); // 自キャラの衝突時コールバックを呼び出す
		enemy_->OnCollision(player_); // 敵の衝突時コールバックを呼び出す
	}
#pragma endregion
}
