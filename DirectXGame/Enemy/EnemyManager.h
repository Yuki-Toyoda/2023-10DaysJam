#pragma once
#include "ViewProjection.h"
#include <Model.h>
#include <vector>

#include "Enemy.h"
#include "EnemyBullet.h"
#include "BossEnemy.h"
#include "EnemySpawner.h"

// プレイヤー
class Player;

/// <summary>
/// エネミーマネージャー
/// </summary>
class EnemyManager {

public: // 静的メンバ関数
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static EnemyManager* GetInstance();

public: // メンバ変数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*>& models, std::vector<uint32_t> textureHandles);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// コライダーの描画
	/// </summary>
	void ColliderDraw();

	/// <summary>
	/// エネミー追加
	/// </summary>
	void AddEnemy(Vector3 position, Enemy::EnemyType enemyTypeNext);

	/// <summary>
	/// エネミー削除
	/// </summary>
	void DeleteEnemy();

	/// <summary>
	/// エネミーバレット追加
	/// </summary>
	void AddEnemyBullet(const Vector3& position,const Vector3& velocity);

	/// <summary>
	/// エネミーバレット削除
	/// </summary>
	void DeleteEnemyBullet();

	/// <summary>
	/// ボスエネミー追加
	/// </summary>
	void AddBossEnemy();

	/// <summary>
	/// エネミースポナー追加
	/// </summary>
	void AddEnemySpawner();

	/// <summary>
	///	削除
	/// </summary>
	void Delete();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// エネミーゲッター
	/// </summary>
	std::list<Enemy*> GetEnemies() { return enemies_; }

	/// <summary>
	/// ボスエネミーゲッター
	/// </summary>
	/// <returns></returns>
	std::list<BossEnemy*> GetBossEnemis() { return bossEnemies_; }

	/// <summary>
	/// エネミーカウントゲッター
	/// </summary>
	/// <returns></returns>
	size_t GetEnemyCount() { return enemyCount_; }

	/// <summary>
	/// エネミーカウントセッター
	/// </summary>
	/// <param name="enemyCount"></param>
	void SetEnemyCount(size_t enemyCount) { enemyCount_ = enemyCount; }

	/// <summary>
	/// プレイヤーセッター
	/// </summary>
	void SetPlayer(Player* player) { player_ = player; }

private: // その他関数
	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables();

private: // シングルトンパターン
	EnemyManager() = default;
	~EnemyManager() = default;
	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;

private: // メンバ変数
	
	// エネミー
	std::list<Enemy*> enemies_;
	// エネミーモデル
	std::vector<Model*> models_;
	// エネミーのテクスチャハンドル
	std::vector<uint32_t> textureHandles_;

	// エネミーバレット
	std::list<EnemyBullet*> enemyBullets_;
	// エネミーバレットモデル
	std::vector<Model*> bulletModels_;

	// ボスエネミー
	std::list<BossEnemy*> bossEnemies_;
	// ボスエネミーモデル
	std::vector<Model*> bossModels_;
	// ボスエネミーのテクスチャハンドル
	uint32_t bossTextureHandles_ = 0u;

	// エネミー数
	size_t enemyCount_;
	// エネミー最大数
	uint32_t enemyMax = 1;

	// プレイヤー
	Player* player_ = nullptr;

	// エネミースポナー
	std::list<EnemySpawner*> enemySpawneres_;

	//ボスエネミーのコライダーサイズ
	Vector3 bossEnemyColliderSize_;

	//スポナーナンバー
	uint32_t spawnerNumber_;

	//初期Hp
	uint32_t initialHp_[4];
	//ボス初期Hp
	uint32_t bossInitialHp_;

};
