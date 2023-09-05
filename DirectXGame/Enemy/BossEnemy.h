#pragma once
#include "../Charactor/BaseCharacter.h"

// プレイヤークラス前方宣言
class Player;

/// <summary>
/// ボスエネミー
/// </summary>
class BossEnemy : public BaseCharacter {

	public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデルデータ配列</param>
	void Initialize(const std::vector<Model*>& models, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(const ViewProjection& viewProjection) override;

	// 衝突時に呼ばれる関数
	void OnCollision() override;

	// 死んでいるか
	bool GetIsDead() { return isDead_; }

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

private: // メンバ変数
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// デスフラグ
	bool isDead_ = false;

	// 速度
	Vector3 velocity_ = {
	    0.0f,
	    0.0f,
	    0.0f,
	};

	//プレイヤー
	Player* player_;

	// 移動

	//速さ
	float approachThePlayerSpeed = 1.0f;
	float moveAwayFromPlayerSpeed = -1.0f;
	float playerSatelliteSpeed = 1.0f;

	//適正距離
	float appropriateDistance = 80.0f;
	//距離の許容範囲
	float distanceTolerance = 20.0f;


};
