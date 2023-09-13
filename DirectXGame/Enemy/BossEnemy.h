#pragma once
#include "../Charactor/BaseCharacter.h"
#include "../audio/Audio.h"

// プレイヤークラス前方宣言
class Player;

// エネミークラスの宣言
class Enemy;

// エネミーマネージャークラスの宣言
class EnemyManager;

/// <summary>
/// ボスエネミー
/// </summary>
class BossEnemy : public BaseCharacter {

public: // サブクラス
	enum BossEnemyState { 
		Collect, 
		PreAttackCommand, 
		AttackCommand, 
		Down
	};

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデルデータ配列</param>
	void Initialize(
	    const std::vector<Model*>& models, uint32_t textureHandle, EnemyManager* enemyManager,
	    Player* player, uint32_t hp, const std::vector<uint32_t>& audioHandles,
	    const std::vector<Model*>& explosionModels);

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
	/// 攻撃準備
	/// </summary>
	void PreAttack();

	/// <summary>
	/// 攻撃開始
	/// </summary>
	void BeginAttack();

	/// <summary>
	/// 攻撃中
	/// </summary>
	void UnderAttack();

	/// <summary>
	/// 移動中の回転
	/// </summary>
	void MoveRotation(Vector3 toPosition);

	/// <summary>
	/// 射撃攻撃
	/// </summary>
	void ShotAttack();

	/// <summary>
	/// 死んだ
	/// </summary>
	void Dead();

	/// <summary>
	/// HP変動
	/// </summary>
	void HpFluctuation(int32_t damage, uint32_t InvincibilityTime);

	/// <summary>
	/// 位置制限
	/// </summary>
	void TranslationLimit();

	/// <summary>
	/// モデルを揺らす開始
	/// </summary>
	void PlayModelShake(Vector3 shakeStrength, float shakeTime);

	/// <summary>
	/// モデルを揺らす
	/// </summary>
	Vector3 ModelShake();

	/// <summary>
	/// ダウンムーブ
	/// </summary>
	void DownMove();

public: // 衝突処理

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
	void CollisionBulletIce(Collider* collision);

	/// <summary>
	/// 雷弾
	/// </summary>
	void CollisionBulletThunder();

	/// <summary>
	/// エネミー
	/// </summary>
	void CollisionEnemy();

	/// <summary>
	/// プレイヤー
	/// </summary>
	void CollisionPlayer();

public: //アクセッサ

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

	/// <summary>
	/// 状態取得
	/// </summary>
	/// <returns></returns>
	BossEnemyState GetBossEnemyState() { return bossEnemyState_; }

	/// <summary>
	/// 追従エネミートランスフォーム取得
	/// </summary>
	Vector3* GetUnitTtransform() { return unitTtransform_; }

	/// <summary>
	/// 無敵かゲッター
	/// </summary>
	/// <returns></returns>
	bool GetIsInvincible() { return isInvincible_; }

	/// <summary>
	/// hpゲッター
	/// </summary>
	/// <returns></returns>
	int32_t GetHp() { return hp_; }

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

	//エネミーマネージャー
	EnemyManager* enemyManager_ = nullptr;

	// 移動
	float moveSpeed_ = 1.0f;
	float moveRotateSpeed_ = 0.01f;

	// 弾速度
	float bulletSpeed_ = 1.5f;

	// エネミー
	std::list<Enemy*>* enemies_;

	//ボスエネミー状態
	BossEnemyState bossEnemyState_;

	//エネミーの数
	uint32_t enemiesJoiningNum;

	//エネミーの最大数
	uint32_t enemiesJoiningNumMax;

	//攻撃準備
	//	回転用のT
	float preAttackT_;
	//	回転用のT
	float preAttackTSpeed_ = 0.05f;
	// クールタイム
	uint32_t preAttackCooltime_;
	// クールタイム最大
	uint32_t preAttackCooltimeMax_ = 60 * 2;

	//攻撃指示
	//クールタイム
	uint32_t attackCooltime_;
	// クールタイム最大
	uint32_t attackCooltimeMax_ = 60 * 2;

	//部隊
	static const uint32_t kUnitPattern = 1;
	static const uint32_t kUnitTransformnumMax = 20;

	//その部隊パターンのエネミーの数
	uint32_t unitTransformNum_[kUnitPattern];
	//座標
	Vector3 unitTtransformData_[kUnitPattern][kUnitTransformnumMax];

	//どのパターンか
	uint32_t unitPattern_;
	// 座標
	Vector3 unitTtransform_[kUnitTransformnumMax];

	// 体力
	int32_t hp_;

	// 無敵か
	bool isInvincible_;

	// 無敵タイマー
	uint32_t invincibilityTimer_;

	// 衝突無敵タイマー
	uint32_t collisionInvincibilityTime_;

	// 射撃クールタイム
	uint32_t shotAttackCooltime_;

	// 最大射撃クールタイム
	uint32_t shotAttackCooltimeMax_;

	// 雷弾無敵タイマー
	uint32_t thunderInvincibilityTime_;

	// 前フレームの位置
	Vector3 preTranslation_ = {0.0f, 0.0f, 0.0f};
	// 氷の跳ね返り加速
	Vector3 accelerationIce_ = {0.0f, 0.0f, 0.0f};
	// 氷の跳ね返り加速量ダウン
	Vector3 accelerationIceDown_ = {0.0f, 0.0f, 0.0f};

	// サンダー
	bool isCollisionThunder = false;
	// 減速倍率
	float decelerationMagnification = 0.5f;

	// 振動強さ
	Vector3 shakeStrength_;
	// シェイク有効トリガー
	bool enableShake_;
	// シェイク演出用t
	float shakeT_;
	// シェイク演出時間
	float shakeTime_;

	// モデル
	WorldTransform modelWorldTransform_;

	//効果音リスト
	std::vector<uint32_t> audioHandles_;
	
	// オーディオ
	Audio* audio_;

	//爆破モデル
	std::vector<Model*> explosionModels_;

	WorldTransform explosionWorldTransform_[3];

	uint32_t explosionTimer_;
	
	uint32_t explosionTimerStart[3] = {0, 30, 60};
	uint32_t explosionTimerend[3] = {60, 90, 120};

	bool isExplosion_[3];


};
