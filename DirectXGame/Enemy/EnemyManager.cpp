#include "EnemyManager.h"
#include "../config/GlobalVariables.h"

EnemyManager* EnemyManager::GetInstance() {
	static EnemyManager instance;
	return &instance;
}

void EnemyManager::Initialize(const std::vector<Model*>& models) {
	
	//モデル
	models_ = models;
	// エネミーのテクスチャハンドル
	//textureHandle_ = textureHandle; 
	
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "EnemyManager";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加
	globalVariables->AddItem(groupName, "EnemyMax",  int(enemyMax));
	
	//エネミーの数
	enemyCount_ = enemyMax;

	//バレットモデル
	bulletModels_ = models;

	//ボスエネミーモデル
	//bossModels_ = models;

	//初期のエネミー配置
	for (size_t i = 0; i < enemyMax; i++) {
		AddEnemy();
	}

	//ボス
	//bossEnemy_ = std::make_unique<BossEnemy>();
	//bossEnemy_->Initialize(bossModels_, textureHandle_);

}

void EnemyManager::Update() {

	//ボスエネミー更新
	//bossEnemy_->Update();
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

	// ボスエネミーの描画
	//bossEnemy_->Draw(viewProjection);
	
	//エネミーの描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection);
	}
	// バレット描画
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Draw(viewProjection);
	}

}

void EnemyManager::ColliderDraw() {

	// エネミーの描画
	for (Enemy* enemy : enemies_) {
		enemy->GetColliderShape()->Draw();
	}
	// バレット描画
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->GetColliderShape()->Draw();
	}

}

void EnemyManager::AddEnemy() {
	
	Enemy* enemy = new Enemy();

	enemy->Initialize(models_, textureHandle_);
	enemy->SetEnemyManager(this);
	enemy->SetPlayer(player_);
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

void EnemyManager::ApplyGlobalVariables() {

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "EnemyManager";

	// メンバ変数の調整項目をグローバル変数に追加
	enemyMax = uint32_t(globalVariables->GetIntValue(groupName, "EnemyMax"));

}