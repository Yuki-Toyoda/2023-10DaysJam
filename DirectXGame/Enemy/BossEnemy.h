#pragma once
#include "../Charactor/BaseCharacter.h"

// プレイヤークラス前方宣言
class Player;

// エネミークラスの宣言
class Enemy;

/// <summary>
/// ボスエネミー
/// </summary>
class BossEnemy : public BaseCharacter {

public: // サブクラス
	enum BossEnemyState { 
		Collect,
		AttackCommand, 
		Down
	};

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデルデータ配列</param>
	void Initialize(const std::vector<Model*>& models, uint32_t textureHandle, std::list<Enemy*>* enemies, Vector3 colliderSize);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(std::list<Enemy*>* enemies);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(const ViewProjection& viewProjection) override;

	// 衝突時に呼ばれる関数
	void OnCollision(Collider* collision) override;

	// 死んでいるか
	bool GetIsDead() { return isDead_; }

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// エネミーを回収する
	/// </summary>
	void CollectEnemies();

	/// <summary>
	/// 移動中の回転
	/// </summary>
	void MoveRotation(Vector3 toPosition);

	/// <summary>
	/// プレイヤーセッター
	/// </summary>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// エネミーの数ゲッター
	/// </summary>
	/// <returns></returns>
	uint32_t GetEnemiesJoiningNum() { return enemiesJoiningNum; }

	/// <summary>
	/// エネミーの最大数ゲッター
	/// </summary>
	/// <returns></returns>
	uint32_t GetEnemiesJoiningNumMax() { return enemiesJoiningNumMax; }

public: // 自分の情報
	/// <summary>
	/// 自分の情報取得
	/// </summary>
	/// <returns></returns>
	BossEnemy* GetBossEnemy() override { return this; }

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
	float moveSpeed_ = 1.0f;
	float moveRotateSpeed_ = 0.01f;

	// エネミー
	std::list<Enemy*>* enemies_;

	//ボスエネミー状態
	BossEnemyState bossEnemyState_;

	//エネミーの数
	uint32_t enemiesJoiningNum;

	//エネミーの最大数
	uint32_t enemiesJoiningNumMax;

};
