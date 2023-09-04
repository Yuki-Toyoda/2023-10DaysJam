#pragma once
#include "../Charactor/BaseCharacter.h"

//エネミーマネージャーの前方宣言
class EnemyManager;

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
	/// エネミーマネージャー取得
	/// </summary>
	void SetEnemyManager(EnemyManager* enemyManager) { enemyManager_ = enemyManager; }

private: //メンバ変数

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// デスフラグ
	bool isDead_ = false;

	//速度
	Vector3 velocity_ = {
	    0.0f,
	    0.0f,
	    0.0f,
	};

	EnemyManager* enemyManager_ = nullptr;

};
