#include "EnemyManager.h"

EnemyManager* EnemyManager::GetInstance() {
	static EnemyManager instance;
	return &instance;
}

void EnemyManager::Initialize(const std::vector<Model*>& models) {
	
	//モデル
	models_ = models;
	// エネミーのテクスチャハンドル
	//textureHandle_ = textureHandle; 
	//エネミーの数
	enemyCount_ = enemyMax;

	//バレットモデル
	bulletModels_ = models;

	//初期のエネミー配置
	for (size_t i = 0; i < enemyMax; i++) {
		AddEnemy();
	}
}

void EnemyManager::Update() {

	//エネミー更新
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}
	// バレット更新
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Update();
	}

}

void EnemyManager::Draw(const ViewProjection& viewProjection) {

	//エネミーの描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection);
	}
	// バレット描画
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Draw(viewProjection);
	}

}

void EnemyManager::AddEnemy() {
	
	Enemy* enemy = new Enemy();

	enemy->Initialize(models_, textureHandle_);
	enemy->SetEnemyManager(this);
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

void EnemyManager::AddEnemyBullet(const Vector3& position, const Vector3& velocity) {

	// リストに登録する
	EnemyBullet* enemyBullet = new EnemyBullet();
	enemyBullet->Initialize(bulletModels_, position, velocity);
	enemyBullets_.push_back(enemyBullet);

}

void EnemyManager::DeleteEnemyBullet() {

	// デスフラグの立った敵弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
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
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		delete bullet;
		return true;
	});

}

void EnemyManager::Reset() {

	if (enemyCount_ == 0) {
		Delete();
		enemyCount_ = enemyMax;

		for (size_t i = 0; i < enemyMax; i++) {
			AddEnemy();
		}
	}

}