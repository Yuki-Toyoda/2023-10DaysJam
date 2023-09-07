#include "EnemySpawner.h"
#include "EnemyManager.h"
#include <config/GlobalVariables.h>

void EnemySpawner::Initialize(EnemyManager* enemyManager) {

	generateCooltime_ = kGenerateCooltime_;

	enemyManager_ = enemyManager;

	position_ = Vector3(0.0f,0.0f,0.0f);

	size_ = {10.0f,0.0f,10.0f};

	posChangeCooltime_ = 0;

	posChangeNum = 1; 
	
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "EnemySpawner";

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

void EnemySpawner::ApplyGlobalVariables() {

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "EnemySpawner";

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

}
