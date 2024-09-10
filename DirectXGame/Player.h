#pragma once
#include "AABB.h"
#include "MatrixFunction.h"
#include "Model.h"
#include "Novice.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"
#include <Audio.h>

class MapChipField;

class Enemy;

class Player {

private:
	struct CollisionMapInfo {
		bool ceiling = false; // 天井衝突フラグ
		bool landing = false;
		bool hitWall = false;
		Vector3 move;

		//(bool ceiling, bool landing, bool hitWall, const Vector3& move) : ceiling(ceiling), landing(landing), hitWall(hitWall), move(move) {}
	};

	enum Corner {
		kRightBottom,
		kLeftBottom,
		kRightTop,
		kLeftTop,
		kNumCorner // 要素数
	};
	// 衝突情報を初期化
	// CollisionMapInfo info_;

public:
	void initialize(Model* model, ViewProjection* viewProjection, const Vector3& position, Audio* audio);
	Player();
	~Player();
	void Update();
	void Draw();
	float EaseInOut(float y);
	WorldTransform* GetWorldTransform();
	const Vector3& GetVelocity() const { return velocity_; };                          // 自キャラの速度を取得するためのgetter
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; } // setter
	void Move();
	//	void CollisionMapCheck(CollisionMapInfo& info); // マップ衝突判定
	void CollisionMapCheckUp(CollisionMapInfo& info); // 衝突判定上方向
	void CollisionMapCheckDown(CollisionMapInfo& info);
	void CollisionMapCheckLeft(CollisionMapInfo& info);
	void CollisionMapCheckRight(CollisionMapInfo& info);
	Vector3 CornerPosition(const Vector3& center, Corner corner);
	void MoveByCollisionResult(const CollisionMapInfo& info);
	void CollisionCeilingCase(const CollisionMapInfo& info); // 天井に接触している場合の処理
	void CollisionHitWallCase(const CollisionMapInfo& info); // 壁に接触している場合の処理
	void GroundStateChange(const CollisionMapInfo& info);    // 接地状態の処理
	//	void cornerDetect(const MapChipType& mapChipType,const int& corner);//4角の判定
	
	AABB GetAABB();                       // AABB取得関数
	void OnCollision(const Enemy* enemy); // 衝突応答
	// デスフラグのgetter
	bool IsDeadGetter();
	void CollisionMapCheckAllDirection(CollisionMapInfo& info); // 全ての衝突判定関数を呼び出す
	Vector3 GetWorldPosition();

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t txHandle_ = 0u;
	ViewProjection* viewProjection_ = nullptr;

	Vector3 velocity_ = {0.0f, 0.0f, 0.0f};
	Vector3 vectorBlank = {0, -0.3f, 0};

	// サウンドデータ
	Audio* audio_ = nullptr;
	uint32_t jumpSound = 0;

	MapChipField* mapChipField_ = nullptr;

	enum class LRDirection {
		kRight,
		kLeft,
	};
	LRDirection lrDirection_ = LRDirection::kRight; // 右向きで初期化

	static inline const float kAcceleration = 0.04f;
	static inline const float kAttenuation = 0.4f;     // 速度減衰率
	static inline const float kWallAttenuation = 0.5f; // 速度減衰率(wall)
	static inline const float kLimitRunSpeed = 0.1f;   // 最大速度制限
	static inline const float kBlank = 0.1f;
	float turnFirstRotationY_ = 0.0f;                       // 旋回開始時の角度
	float turnTimer_ = 0.0f;                                // 旋回タイマー
	static inline const float kTimeTurn = 0.3f;             // 旋回時間<秒>
	bool onGround_ = true;                                  // 設置状態フラグ
	static inline const float kGravityAcceleration = 0.04f; // 重力加速度(下方向)
	static inline const float kLimitFallSpeed = 0.45f;      // 最大落下速度(下方向)
	static inline const float kJumpAcceleration = 0.7f;     // ジャンプ初速(上方向）
	static inline const float kGroundPos = 2.0f;            // 地面の座標
	bool landing = false;
	static inline const float kWidth = 1.8f; // キャラクターの当たり判定サイズ
	static inline const float kHeight = 1.8f;
	static inline const float kAttenuationLanding = 0.5f; // 着地時の速度減衰率
	bool isDead_ = false;
};