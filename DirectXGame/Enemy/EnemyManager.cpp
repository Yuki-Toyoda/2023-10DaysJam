#include "EnemyManager.h"

EnemyManager* EnemyManager::GetInstance() {
	static EnemyManager instance;
	return &instance;
}

void EnemyManager::Initialize(const std::vector<Model*>& models, uint32_t textureHandle) {
	
	//���f��
	models_ = models;
	// �G�l�~�[�̃e�N�X�`���n���h��
	textureHandle_ = textureHandle; 
	//�G�l�~�[�̐�
	enemyCount_ = enemyMax;


	//�����̃G�l�~�[�z�u
	for (size_t i = 0; i < enemyMax; i++) {
		AddEnemy(i);
	}
}

void EnemyManager::Update() {

	//�G�l�~�[�X�V
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

}

void EnemyManager::Draw(const ViewProjection& viewProjection) {

	//�G�l�~�[�̕`��
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
