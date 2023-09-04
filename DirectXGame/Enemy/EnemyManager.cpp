#include "EnemyManager.h"

EnemyManager* EnemyManager::GetInstance() {
	static EnemyManager instance;
	return &instance;
}

void EnemyManager::Initialize(const std::vector<Model*>& models, uint32_t textureHandle) {
	
	//モデル
	models_ = models;
	// エネミーのテクスチャハンドル
	textureHandle_ = textureHandle; 
	//エネミーの数
	enemyCount_ = enemyMax;


	//初期のエネミー配置
	for (size_t i = 0; i < enemyMax; i++) {
		AddEnemy(i);
	}
}

void EnemyManager::Update() {

	//エネミー更新
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

}

void EnemyManager::Draw(const ViewProjection& viewProjection) {

	//エネミーの描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection);
	}

}

void EnemyManager::AddEnemy(size_t positionNum) {
	
	Enemy* enemy = new Enemy();

	enemy->Initialize(models_, textureHandle_);
	enemies_.push_back(enemy);

}

void EnemyManager::DeleteEnemy() {

	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->GetIsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

}

void EnemyManager::Delete() {

	enemies_.remove_if([](Enemy* enemy) {
		delete enemy;
		return true;
	});

}

void EnemyManager::Reset() {

	if (enemyCount_ == 0) {
		Delete();
		enemyCount_ = enemyMax;

		for (size_t i = 0; i < enemyMax; i++) {
			AddEnemy(i);
		}
	}

}
