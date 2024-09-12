#pragma once

#include "2Dspr.h"
#include "Audio.h"
#include "CameraController.h"
#include "DeathParticles.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Goal.h"
#include "Input.h"
#include "MapChipField.h"
#include "Model.h"
#include "Novice.h"
#include "Player.h"
#include "SkyDome.h"
#include "Sprite.h"
#include "Vector4.h"
#include "ViewProjection.h"
#include <vector>
/// <summary>
/// ゲームシーン
/// </summary>

enum class Phase {
	kPlay,
	kDeath,
};

class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 表示ブロックの生成
	/// </summary>
	void GenerateBlocks();

	// 全ての当たり判定を行う
	void CheckAllCollisions();

	// フェーズの切り替え
	void ChangePhase();

	// 終了フラグのgetter
	bool IsFinished() const { return IsFinished_; }

	void BlocksUpdate();

	void CameraUpdate();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	uint32_t playerTxHandle_ = 0u;
	Model* modelPlayer_ = nullptr; // 自キャラ3Dmodel
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	ViewProjection cameraViewProjection_; // カメラ用
	Player* player_ = nullptr;
	DeathParticles* deathParticles_ = nullptr;
	Model* modelParticles_ = nullptr;
	Enemy* enemy_ = nullptr;
	Model* modelEnemy = nullptr;
	uint32_t enemyTxhandle = 0u;

	uint32_t BGM = 0;
	Model* modelBlock_ = nullptr;                                    // ブロック
	uint32_t blockTxHandle_ = 0u;                                    // ブロックテクスチャハンドル
	uint32_t spriteHandle_ = 0u;                                     // 2Dテクスチャハンドル
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_; // 可変個配列

	bool isDebugCameraactive_ = false;
	CameraController* cameraController_ = nullptr; // カメラコントローラー

	SkyDome* skyDome_ = nullptr;
	Model* modelSkyDome_ = nullptr;
	MapChipField* mapChipField_; // マップチップフィールド

	MatrixFunction* matrixFunction = nullptr;
	Phase phase_;

	Box* box_ = nullptr;

	// ゴール
	Goal* goal_ = nullptr;
	Model* modelGoal_ = nullptr;

	// 終了フラグ
	bool IsFinished_ = false;
};
