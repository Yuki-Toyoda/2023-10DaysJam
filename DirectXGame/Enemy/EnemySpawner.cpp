#include "EnemySpawner.h"
#include "EnemyManager.h"

void EnemySpawner::Initialize(Vector3 position, EnemyManager* enemyManager) {

	// 位置
	position_ = position;
	// 生成クールタイム
	cooltime_ = kCooltime_;
	//　エネミーマネージャー
	enemyManager_ = enemyManager;

}

void EnemySpawner::Update() {

	if (--cooltime_ == 0) {
		// 生成
		Spawn();
		// 生成クールタイム
		cooltime_ = kCooltime_;
	}

}

void EnemySpawner::Spawn() {

	enemyManager_->AddEnemy(position_);

}
