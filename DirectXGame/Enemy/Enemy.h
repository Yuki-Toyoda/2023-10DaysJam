#pragma once
#include "../Charactor/BaseCharacter.h"

//エネミーマネージャーの前方宣言
class EnemyManager;
//プレイヤー
class Player;
//ボスエネミー
class BossEnemy;

/// <summary>
/// エネミークラス
/// </summary>
class Enemy : public BaseCharacter {

public: //サブクラス
	//  属性の種類
	enum EnemyType {
		None,   // 通常
		Fire,   // 炎
		Ice,    // 氷
		Thunder // 雷
	};

	// 状態
	enum EnemyState {
		Wait,
		Follow,
		PreRush,
		Rush
	};


public: //メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデルデータ配列</param>
	void Initialize(
	    const std::vector<Model*>& models, uint32_t textureHandle, EnemyType enemyType,
	    Vector3 posioton, uint32_t hp, EnemyManager* enemyManager, Player* player,
	    std::list<BossEnemy*>* bossEnemies);

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
	void OnCollision(Collider* collision) override;

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
	/// 突進準備
	/// </summary>
	void PreRushStart();

	/// <summary>
	/// 突進準備
	/// </summary>
	void PreRushing();

	/// <summary>
	/// 突進開始
	/// </summary>
	void RushStart();

	/// <summary>
	/// 突進
	/// </summary>
	void Rushing();

	/// <summary>
	/// 待ち
	/// </summary>
	void Waiting();

	/// <summary>
	/// 追従(ボス)
	/// </summary>
	void Following();

	/// <summary>
	/// 部隊に加わる
	/// </summary>
	void Join(BossEnemy* bossEnemy);

	/// <summary>
	/// 移動中の回転
	/// </summary>
	void MoveRotation(Vector3 toPosition);

	/// <summary>
	/// 死んだ
	/// </summary>
	void Dead();

	/// <summary>
	/// HP変動
	/// </summary>
	void HpFluctuation(int32_t damage, uint32_t InvincibilityTime);

public: //衝突処理
	
	/// <summary>
	/// 通常弾
	/// </summary>
	void CollisionBulletNone();

	/// <summary>
	/// 炎弾
	/// </summary>
	void CollisionBulletFire();

	/// <summary>
	/// 氷弾
	/// </summary>
	void CollisionBulletIce();

	/// <summary>
	/// 雷弾
	/// </summary>
	void CollisionBulletThunder();

	/// <summary>
	/// ボスエネミー
	/// </summary>
	void CollisionBossEnemy(BossEnemy* bossEnemy);
	
	/// <summary>
	/// プレイヤー
	/// </summary>
	void CollisionPlayer();

public: //アクセッサ

	/// <summary>
	/// エネミーマネージャーセッター
	/// </summary>
	void SetEnemyManager(EnemyManager* enemyManager) { enemyManager_ = enemyManager; }

	/// <summary>
	/// プレイヤーセッター
	/// </summary>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// エネミーの状態ゲッター
	/// </summary>
	/// <returns></returns>
	EnemyState GetEnemyState() { return enemyState_; }

	/// <summary>
	/// 無敵かゲッター
	/// </summary>
	/// <returns></returns>
	bool GetIsInvincible() { return isInvincible_; }

public: // 自分の情報
	
	/// <summary>
	/// 自分の情報取得
	/// </summary>
	/// <returns></returns>
	Enemy* GetEnemy() override { return this; }

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
	// 速度
	Vector3 velocity_ = {};

	//エネミーマネージャー
	EnemyManager* enemyManager_ = nullptr;
	//プレイヤー
	Player* player_ = nullptr;
	//ボスエネミー
	std::list<BossEnemy*>* bossEnemies_;

	//移動
	float moveSpeed_ = 1.0f;
	float moveRotateSpeed_ = 0.01f;
	//プレイヤーへ移動
	float moveToPlayerSpeed_ = 0.5f;
	float closeToThePlayer_ = 100.0f;
	//弾速度
	float bulletSpeed_ = 0.5f;
	//突進速度
	float rushSpeed_ = 5.0f;

	//エネミータイプ
	EnemyType enemyType_;

	//エネミーの状態
	EnemyState enemyState_;

	// ボスエネミーに追従するときに使う変数
	
	//追従するボス
	BossEnemy* JoiningBossEnemy_ = nullptr;
	// ボスとの距離
	float distanceToBoss_;
	// ボスとの回転角
	Vector3 rotationToBoss_;
	// 目指すワールドトランスフォーム
	WorldTransform targetWorldTransform;

	//突進準備用
	//スタート位置
	Vector3 preRushStartPos_;
	//ゴール位置
	Vector3 preRushEndPos_;
	//T
	float preRushT_;
	// Tの速度
	float preRushTSpeed_ = 0.05f;
	//下がる距離
	float preRushDistance_ = 100.0f;

	//体力
	int32_t hp_;

	//無敵か
	bool isInvincible_;

	//無敵タイマー
	uint32_t invincibilityTimer_;

	// 衝突無敵タイマー
	uint32_t collisionInvincibilityTime_;



};
