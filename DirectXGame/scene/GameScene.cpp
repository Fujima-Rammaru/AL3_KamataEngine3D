#include "GameScene.h"
#include "DebugCamera.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
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
	delete mapChipField_;
	delete cameraController_;
	delete modelParticles_;
	delete deathParticles_;
	delete matrixFunction;
	delete light_;
	delete goal_;
	delete modelGoal_;
	delete lightPowItem_;
	delete mdlLightPowItem;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	worldTransform_.Initialize();

	cameraViewProjection_.Initialize();
	PrimitiveDrawer::GetInstance()->SetViewProjection(&cameraViewProjection_);
	matrixFunction = new MatrixFunction;

	// マップチップの生成
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/stage1.csv"); // CSVファイル読み込み

	// ブロックの初期化
	blockTxHandle_ = TextureManager::Load("cube/cube.jpg");
	modelBlock_ = Model::Create();
	GenerateBlocks();

	// プレイヤーの初期化
	modelPlayer_ = Model::CreateFromOBJ("player", true); // 3Dモデルの生成
	player_ = new Player();                              // 自キャラの生成
	playerposition = mapChipField_->GetMapChipPositionByIndex(2, 17);
	player_->initialize(modelPlayer_, &cameraViewProjection_, playerposition, audio_); // 自キャラの初期化
	player_->SetMapChipField(mapChipField_);
	phase_ = Phase::kPlay;

	// パーティクルの初期化
	deathParticles_ = new DeathParticles();
	modelParticles_ = Model::CreateFromOBJ("Particle", true);
	deathParticles_->Initialize(modelParticles_, &cameraViewProjection_, playerposition);

	// 敵キャラの生成
	enemyTxhandle = TextureManager::Load("sample.png"); // テクスチャの読み込み
	modelEnemy = Model::Create();
	enemy_ = new Enemy();
	Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(70, 18);
	enemy_->Initialize(modelEnemy, enemyTxhandle, &cameraViewProjection_, enemyPosition);
	enemy_->SetPlayer(player_);

	// カメラコントローラー初期化
	Rect area_ = {30.0f, 300.0f, 0.0f, 100.0f};
	cameraController_ = new CameraController;
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();
	cameraController_->SetMovableArea(area_);

	// スプライト初期化
	light_ = new Box();
	light_->Initialize();
	lightPos = light_->GetPosition();
	lightSize = light_->GetSize();
	stdLightPos = light_->GetPosition();
	stdLightSize = light_->GetSize();
	// ゴール
	Vector3 goalPos = mapChipField_->GetMapChipPositionByIndex(15, 16);
	goal_ = new Goal();
	modelGoal_ = Model::CreateFromOBJ("Goal", true);
	goal_->Initialize(modelGoal_, &cameraViewProjection_, goalPos);

	// アイテム
	Vector3 ItemPos = mapChipField_->GetMapChipPositionByIndex(10, 14);
	lightPowItem_ = new LightPowItem();
	mdlLightPowItem = Model::CreateFromOBJ("Item", true);
	lightPowItem_->Initialize(mdlLightPowItem, &cameraViewProjection_, ItemPos);

	// サウンド
	BGM = audio_->LoadWave("sound/BGM.mp3");
	if (audio_->IsPlaying(BGM) == false) {
		BGM = audio_->PlayWave(BGM, true, 0.07f);
	}
}
void GameScene::Update() {

	playerWorldT = player_->GetWorldTransform();
	ChangePhase();
	// lightの大きさ変更処理（後で衝突応答による処理に変更）
	if (playerWorldT->translation_.x > 20.0f) {
		if (lightSize.x > 3840 - 560) {
			lightPos.x += 2.f;
			lightPos.y += 1.125f;
			lightSize.x -= 4.f;
			lightSize.y -= 2.25f;
		}
		light_->Setposition(lightPos);
		light_->SetSize(lightSize);
	} // else { // 条件を満たさないなら元に戻す
	// lightPos = stdLightPos;
	// lightSize = stdLightSize;
	// light_->Setposition(lightPos);
	// light_->SetSize(lightSize);
	//}
	light_->Update();

#ifdef _DEBUG
	ImGui::Text("playerTrans.x=%3.2f", playerWorldT->translation_.x);
#endif
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	// Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる

	/// </summary>

	// スプライト描画後処理
	// Sprite::PostDraw();
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

	if (player_->IsDeadGetter() == false) {
		player_->Draw();
	}

	enemy_->Draw();
	if (deathParticles_) {
		deathParticles_->Draw();
	}
	goal_->Draw();
	lightPowItem_->Draw();
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
	light_->Draw(commandList);

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
		IsFinished_ = true;
		player_->OnCollision(enemy_); // 自キャラの衝突時コールバックを呼び出す
		enemy_->OnCollision(player_); // 敵の衝突時コールバックを呼び出す
	}

#pragma endregion 自キャラとゴールの当たり判定
	AABB aabb3, aabb4;
	// 自キャラの座標
	aabb3 = player_->GetAABB();
	// ゴールの座標
	aabb4 = goal_->GetAABB();
	if (aabb4.isHit(aabb3)) {
		goal_->Oncollision(player_);
		audio_->StopWave(BGM);
	}

#pragma region 自キャラとアイテムの当たり判定
	AABB aabb5, aabb6;
	aabb5 = player_->GetAABB();
	aabb6 = lightPowItem_->GetAABB();
	if (aabb6.isHit(aabb5) && !lightPowItem_->IsFinishedGetter()) {
		lightPowItem_->Oncollision(player_);
		lightPos.x *= 2.125f;
		lightPos.y *= 2.125f;
		lightSize.x *= 1.75f;
		lightSize.y *= 1.75f;
		light_->Setposition(lightPos);
		light_->SetSize(lightSize);
	}
#pragma endregion
}

void GameScene::ChangePhase() {

	switch (phase_) {

	case Phase::kPlay:

		BlocksUpdate();

		if (player_->IsDeadGetter() == false) {
			player_->Update();
		}

		enemy_->Update();
		goal_->Update();
		lightPowItem_->Update();
		CameraUpdate();

		CheckAllCollisions();

		// 自キャラがやられたら
		if (player_->IsDeadGetter()) {
			phase_ = Phase::kDeath;
			audio_->StopWave(BGM);
			const Vector3& deathParticlePosition = player_->GetWorldPosition();
			deathParticles_->Initialize(modelParticles_, &cameraViewProjection_, deathParticlePosition);
		}
		break;

	case Phase::kDeath:
		BlocksUpdate();

		enemy_->Update();

		CameraUpdate();

		if (deathParticles_) {
			deathParticles_->Update();
		}

		if (deathParticles_ && deathParticles_->IsFinished()) {
			IsFinished_ = true;
		}
		break;
	}
}

void GameScene::BlocksUpdate() {
	for (auto& worldTransformBlockLine : worldTransformBlocks_) {
		for (auto worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}

			worldTransformBlock->matWorld_ = matrixFunction->MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
			// 定数バッファに転送する
			worldTransformBlock->UpdateMatrix();
		}
	}
}

void GameScene::CameraUpdate() {

	isDebugCameraactive_ = true;

	if (isDebugCameraactive_) {
		cameraController_->Update();
		cameraViewProjection_.matView = cameraController_->GetMatView();

		cameraViewProjection_.matProjection = cameraController_->GetMatProjection();

		cameraViewProjection_.TransferMatrix();
	} else {
		cameraViewProjection_.UpdateMatrix();
	}
}

bool GameScene::IsGoaled() { return goal_->IsFinishedGetter(); }
