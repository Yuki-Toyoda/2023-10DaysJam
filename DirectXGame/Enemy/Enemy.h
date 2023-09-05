#pragma once
#include "../Charactor/BaseCharacter.h"

//エネミーマネージャーの前方宣言
class EnemyManager;
//プレイヤー
class Player;

/// <summary>
/// エネミークラス
/// </summary>
class Enemy : public BaseCharacter {

public: //メンバ関数

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

	/// <summary>
	/// プレイヤーへ移動
	/// </summary>
	void MoveToPlayer();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 突進
	/// </summary>
	void Rush();

	/// <summary>
	/// 追従(ボス)
	/// </summary>
	void Follow();

	/// <summary>
	/// エネミーマネージャーセッター
	/// </summary>
	void SetEnemyManager(EnemyManager* enemyManager) { enemyManager_ = enemyManager; }

	/// <summary>
	/// プレイヤーセッター
	/// </summary>
	void SetPlayer(Player* player) { player_ = player; }

private: // その他関数
	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables() override;

private: //メンバ変数

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// デスフラグ
	bool isDead_ = false;

	//エネミーマネージャー
	EnemyManager* enemyManager_ = nullptr;
	//プレイヤー
	Player* player_ = nullptr;

	//移動
	float moveSpeed_ = 1.0f;
	float moveRotateSpeed_ = 0.01f;
	//プレイヤーへ移動
	float moveToPlayerSpeed_ = 0.5f;
	//弾速度
	float bulletSpeed_ = 0.5f;


};
