#include "EnemySpawner.h"
#include "EnemyManager.h"
#include <config/GlobalVariables.h>

void EnemySpawner::Initialize(EnemyManager* enemyManager, uint32_t num) {

	generateCooltime_ = generateCooltimeMax_;

	enemyManager_ = enemyManager;

	position_ = Vector3(0.0f,0.0f,0.0f);

	size_ = {10.0f,0.0f,10.0f};

	posChangeCooltime_ = 0;

	posChangeNum = 1; 
	
	num_ = num;

	enemyTypeNext_ = Enemy::EnemyType::None;
	
	specialTypeRatio = 0.1f;

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	char groupName[32];
	sprintf_s(groupName, "EnemySpawner%d", int(num_));

	for (size_t i = 0; i < MoveNumberofTimes; i++) {
		char str[32];
		sprintf_s(str, "position%d", int(i));
		globalVariables->AddItem(groupName, str, posChange[i].position);
	}
	for (size_t i = 0; i < MoveNumberofTimes; i++) {
		char str[32];
		sprintf_s(str, "posChangeCooltime%d", int(i));
		globalVariables->AddItem(groupName, str, int(posChange[i].posChangeCooltime));
	}

	globalVariables->AddItem(groupName, "GenerateCooltimeMax", int(generateCooltimeMax_));

	globalVariables->AddItem(groupName, "Size", size_);

	//移動位置
	ApplyGlobalVariables();
	
	position_ = posChange[0].position;

	posChangeCooltime_ = posChange[0].posChangeCooltime;

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

	//位置
	Vector3 pos = {
	    MyMath::RandomF(position_.x - size_.x, position_.x + size_.x, 2),
	    MyMath::RandomF(position_.y - size_.y, position_.y + size_.y, 2),
	    MyMath::RandomF(position_.z - size_.z, position_.z + size_.z, 2),
	};

	//属性
	float type = MyMath::RandomF(0.0f, 1.0f, 2);
	if (type <= specialTypeRatio) {
		enemyTypeNext_ = Enemy::Fire;
	} else if (type <= specialTypeRatio * 2.0f) {
		enemyTypeNext_ = Enemy::Ice;
	} else if (type <= specialTypeRatio * 3.0f) {
		enemyTypeNext_ = Enemy::Thunder;
	} else {
		enemyTypeNext_ = Enemy::None;
	}

	enemyManager_->AddEnemy(pos,enemyTypeNext_);
	// 生成クールタイム
	generateCooltime_ = generateCooltimeMax_;

}

void EnemySpawner::Move() {

	position_ = posChange[posChangeNum].position;

	posChangeCooltime_ = posChange[posChangeNum].posChangeCooltime;

	posChangeNum++;
	
	if (posChangeNum == MoveNumberofTimes) {
		posChangeNum = 0;
	}

}

void EnemySpawner::ApplyGlobalVariables() {

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	char groupName[32];
	sprintf_s(groupName, "EnemySpawner%d", int(num_));

	// メンバ変数の調整項目をグローバル変数に追加
	for (size_t i = 0; i < MoveNumberofTimes; i++) {
		char str[32];
		sprintf_s(str, "position%d", int(i));
		posChange[i].position = globalVariables->GetVector3Value(groupName, str);
	}
	for (size_t i = 0; i < MoveNumberofTimes; i++) {
		char str[32];
		sprintf_s(str, "posChangeCooltime%d", int(i));
		posChange[i].posChangeCooltime = globalVariables->GetIntValue(groupName, str);
	}

	generateCooltimeMax_ = uint32_t(globalVariables->GetIntValue(groupName, "GenerateCooltimeMax"));

	size_ = globalVariables->GetVector3Value(groupName, "Size");

}
