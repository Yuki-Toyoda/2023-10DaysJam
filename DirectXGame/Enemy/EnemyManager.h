#pragma once
#include "ViewProjection.h"
#include <Model.h>
#include <vector>

#include "Enemy.h"
#include "EnemyBullet.h"

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
	void Initialize(const std::vector<Model*>& models);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// エネミー追加
	/// </summary>
	void AddEnemy();

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
	///
	/// </summary>
	std::list<Enemy*> GetEnemies() { return enemies_; }

	size_t GetEnemyCount() { return enemyCount_; }

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
	uint32_t textureHandle_ = 0u; 

	// エネミーバレット
	std::list<EnemyBullet*> enemyBullets_;
	// エネミーバレットモデル
	std::vector<Model*> bulletModels_;

	// エネミー数
	size_t enemyCount_;
	// エネミー最大数
	const size_t enemyMax = 1;
};