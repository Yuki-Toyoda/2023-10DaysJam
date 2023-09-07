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
	void OnCollision(Tag collisionTag) override;

	// 死んでいるか
	bool GetIsDead() { return isDead_; }

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 回転
	/// </summary>
	void Rotation();

	/// <summary>
	/// プレイヤーセッター
	/// </summary>
	void SetPlayer(Player* player) { player_ = player; }

private: // その他関数
	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables() override;

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
	Player* player_ = nullptr;

	// 移動

	// 移動通過地点数
	uint32_t movePointMax = 4;
	// 移動通過地点番号
	uint32_t movePointNum = 0;
	
	// 移動通過地点
	Vector3 movePoint[4];

	// 移動用T
	float moveT = 0.0f;
	// 移動時間
	float moveTime = 10.0f;

};
