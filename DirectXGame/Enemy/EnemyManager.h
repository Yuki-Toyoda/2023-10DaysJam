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
	void AddEnemy(Vector3 position);

	/// <summary>
	/// エネミー削除
	/// </summary>
	void DeleteEnemy();

	/// <summary>
	/// エネミー追加
	/// </summary>
	void AddEnemyBullet(const Vector3& position,const Vector3& velocity);

	/// <summary>
	/// エネミー削除
	/// </summary>
	void DeleteEnemyBullet();

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
	/// エネミーカウントゲッター
	/// </summary>
	/// <returns></returns>
	size_t GetEnemyCount() { return enemyCount_; }

	/// <summary>
	/// プレイヤーセッター
	/// </summary>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// ボスエネミープレイヤーセッター
	/// </summary>
	/// <returns></returns>
	//void SetBossEnemyPlayer(Player* player) { bossEnemy_->SetPlayer(player); }

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
	//std::unique_ptr<BossEnemy> bossEnemy_;
	// ボスエネミーモデル
	//std::vector<Model*> bossModels_;

	// エネミー数
	size_t enemyCount_;
	// エネミー最大数
	uint32_t enemyMax = 1;

	// プレイヤー
	Player* player_ = nullptr;

	//次のエネミータイプ
	Enemy::EnemyType enemyTypeNext;

	// エネミースポナー
	EnemySpawner enemySpawner;

};
