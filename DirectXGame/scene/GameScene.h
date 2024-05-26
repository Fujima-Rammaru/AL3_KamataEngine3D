#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>
#include "DebugCamera.h"
#include "SkyDome.h"
#include "MapChipField.h"

/// <summary>
/// ゲームシーン
/// </summary>
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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	uint32_t txHandle_ = 0u;
	Model* modelPlayer_ = nullptr;//自キャラ
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	Player* player_ = nullptr;

	Model* block_ = nullptr;//ブロック
	uint32_t blockTxHandle_ = 0u;//ブロックテクスチャハンドル
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;//可変個配列

	bool isDebugCameraactive_ = false;
	DebugCamera* debugCamera_ = nullptr;
	SkyDome* skyDome_ = nullptr;
	Model* modelSkyDome_ = nullptr;
	MapChipField* mapChipField_;//マップチップフィールド
	/// <summary>
	/// ゲームシーン用
	/// </summary>
	/// 
	///if(velocity_.x*velocity_x<0.001f){velocity_.x}
};
