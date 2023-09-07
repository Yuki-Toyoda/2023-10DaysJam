#include "EnemySpawner.h"
#include "EnemyManager.h"

void EnemySpawner::Initialize(EnemyManager* enemyManager) {

	generateCooltime_ = kGenerateCooltime_;

	enemyManager_ = enemyManager;
	
	for (size_t i = 0; i < MoveNumberofTimes; i++) {
		posChange[i].position = {0.0f, 0.0f, i * 10.0f};
		posChange[i].posChangeCooltime = 60 * 4;
	}

	position_ = posChange[0].position;

	size_ = {10.0f,0.0f,10.0f};

	posChangeCooltime_ = posChange[0].posChangeCooltime;

	posChangeNum = 1; 

}

void EnemySpawner::Update() {

	if (--generateCooltime_ == 0) {
		Spawn();
	}

	if (--posChangeCooltime_ == 0) {
		Move();
	}


}

void EnemySpawner::Spawn() {

	enemyManager_->AddEnemy(position_);
	// 生成クールタイム
	generateCooltime_ = kGenerateCooltime_;

}

void EnemySpawner::Move() {

	position_ = posChange[posChangeNum].position;

	posChangeCooltime_ = posChange[posChangeNum].posChangeCooltime;

	posChangeNum++;
	
	if (posChangeNum == MoveNumberofTimes) {
		posChangeNum = 0;
	}

}
