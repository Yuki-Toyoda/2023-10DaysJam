#include "EnemyManager.h"
#include "../config/GlobalVariables.h"
#include <WinApp.h>

EnemyManager* EnemyManager::GetInstance() {
	static EnemyManager instance;
	return &instance;
}

void EnemyManager::Initialize(
    const std::vector<Model*>& models, std::vector<uint32_t> textureHandles,
    const std::vector<Model*>& bossModels, const std::vector<Model*>& bulletModels,
    const std::vector<Model*>& deathEffectModels, 
	const std::vector<Sprite*>& bossSprites,
    const std::vector<uint32_t>& enemyAudioHandles,
    const std::vector<uint32_t>& bosEenemyAudioHandles) {
	
	//モデル
	models_ = models;
	// エネミーのテクスチャハンドル
	textureHandles_ = textureHandles; 

	initialHp_[Enemy::EnemyType::None] = 2;
	initialHp_[Enemy::EnemyType::Fire] = 2;
	initialHp_[Enemy::EnemyType::Ice] = 2;
	initialHp_[Enemy::EnemyType::Thunder] = 2;

	bossInitialHp_ = 120;
	
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "EnemyManager";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加
	globalVariables->AddItem(groupName, "EnemyMax", int(enemyMax));

	globalVariables->AddItem(groupName, "InitialHpNone", int(initialHp_[Enemy::EnemyType::None]));
	globalVariables->AddItem(groupName, "InitialHpFire", int(initialHp_[Enemy::EnemyType::Fire]));
	globalVariables->AddItem(groupName, "InitialHpIce", int(initialHp_[Enemy::EnemyType::Ice]));
	globalVariables->AddItem(groupName, "InitialHpThunder", int(initialHp_[Enemy::EnemyType::Thunder]));
	globalVariables->AddItem(groupName, "BossInitialHp", int(bossInitialHp_));

	
	//エネミーの数
	enemyCount_ = 0;

	//バレットモデル
	bulletModels_ = bulletModels;

	//ボスエネミーモデル
	bossModels_ = bossModels;

	// エネミー死亡エフェクトのモデル
	deathEffectModels_ = deathEffectModels;
	
	// UIスプライト
	// ボスHP
	bossHpSprite_ = bossSprites[0];

	// ボスHPフレーム
	bossHpFrameSprite_ = bossSprites[1];

	//サイズ
	bossHpSpriteSize_ = Vector2(float(WinApp::kWindowWidth) / 2.0f - 200.0f, 30.0f);
	// hpゲージ
	bossHpSprite_->SetSize(bossHpSpriteSize_);
	bossHpSprite_->SetColor(Vector4(0.8f, 0.2f, 0.2f, 1.0f));
	// ボスHPフレーム
	bossHpFrameSprite_->SetSize(bossHpSpriteSize_);
	bossHpFrameSprite_->SetColor(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	
	// ボスHPネーム
	bossHpNameSprite_ = bossSprites[2];
	bossHpNameSprite_->SetSize(
	    Vector2(bossHpNameSprite_->GetSize().x / 2.0f, bossHpNameSprite_->GetSize().y / 2.0f));
	bossHpNameSprite_->SetColor(Vector4(0.8f, 0.2f, 0.2f, 1.0f));

	//効果音
	enemyAudioHandles_ = enemyAudioHandles;
	bossEnemyAudioHandles_ = bosEenemyAudioHandles;

	// エネミースポナーの追加
	spawnerNumber_ = 0;
	AddEnemySpawner();
	AddEnemySpawner();

	//ボスエネミーの追加
	AddBossEnemy();

	ApplyGlobalVariables();


}

void EnemyManager::Update() {

	//スポナー更新
	for (EnemySpawner* enemySponer : enemySpawneres_) {
		enemySponer->Update();
	}
	
	//エネミー更新
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}
	// バレット更新
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Update();
	}
	// ボスエネミー更新
	for (BossEnemy* bossEnemy : bossEnemies_) {
		bossEnemy->Update(&enemies_);

	// hpゲージ
	Vector2 bossHpSpriteSize = {
		    MyMath::Linear(
		        float(bossEnemy->GetHp()) / float(bossInitialHp_), 0.0f, bossHpSpriteSize_.x),
		    bossHpSpriteSize_.y};
		bossHpSprite_->SetSize(bossHpSpriteSize);
	}

	ApplyGlobalVariables();

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
	//ボスエネミーの描画
	for (BossEnemy* bossEnemy : bossEnemies_) {
		bossEnemy->Draw(viewProjection);
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
	// ボスエネミーの描画
	for (BossEnemy* bossEnemy : bossEnemies_) {
		bossEnemy->GetColliderShape()->Draw();
	}

}

void EnemyManager::SpriteDraw() {

	// ボスHPフレーム
	bossHpFrameSprite_->Draw();
	//hpゲージ
	bossHpSprite_->Draw();
	// ネーム
	bossHpNameSprite_->Draw();

}

void EnemyManager::AddEnemy(Vector3 position, Enemy::EnemyType enemyTypeNext, bool isTutorial) {
	
	if (enemyCount_ < enemyMax) {
		Enemy* enemy = new Enemy();

		enemy->Initialize(
		    models_, textureHandles_[enemyTypeNext], enemyTypeNext, position,
		    initialHp_[enemyTypeNext], this, player_, &bossEnemies_, deathEffectModels_, isTutorial,
		    viewProjection_, enemyAudioHandles_);
		enemies_.push_back(enemy);
		enemyCount_++;
	}

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

void EnemyManager::AddBossEnemy() {

	BossEnemy* bossEnemy = new BossEnemy();

	bossEnemy->Initialize(bossModels_, bossTextureHandles_, this, player_,bossInitialHp_,bossEnemyAudioHandles_);
	bossEnemies_.push_back(bossEnemy);

}

void EnemyManager::AddEnemySpawner() {

	EnemySpawner* enemySpawner = new EnemySpawner();
	enemySpawner->Initialize(this, spawnerNumber_);
	spawnerNumber_++;
	enemySpawneres_.push_back(enemySpawner);

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
	bossEnemies_.remove_if([](BossEnemy* bossEnemy) {
		delete bossEnemy;
		return true;
	});
	enemySpawneres_.remove_if([](EnemySpawner* enemySpawner) { 
		delete enemySpawner;
		return true;
		});

}

void EnemyManager::Reset() {

	Delete();
	enemyCount_ = 0;
	
	// エネミースポナーの追加
	spawnerNumber_ = 0;
	AddEnemySpawner();
	AddEnemySpawner();

	//ボスエネミーの追加
	AddBossEnemy();

}

void EnemyManager::ApplyGlobalVariables() {

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "EnemyManager";

	// メンバ変数の調整項目をグローバル変数に追加
	enemyMax = uint32_t(globalVariables->GetIntValue(groupName, "EnemyMax"));

	initialHp_[Enemy::EnemyType::None] =
	    uint32_t(globalVariables->GetIntValue(groupName, "InitialHpNone"));
	initialHp_[Enemy::EnemyType::Fire] =
	    uint32_t(globalVariables->GetIntValue(groupName, "InitialHpFire"));
	initialHp_[Enemy::EnemyType::Ice] =
	    uint32_t(globalVariables->GetIntValue(groupName, "InitialHpIce"));
	initialHp_[Enemy::EnemyType::Thunder] =
	    uint32_t(globalVariables->GetIntValue(groupName, "InitialHpThunder"));
	bossInitialHp_ =
	    uint32_t(globalVariables->GetIntValue(groupName, "BossInitialHp"));

}
