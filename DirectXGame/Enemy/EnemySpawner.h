#pragma once
#include "Vector3.h"

// エネミーマネージャー前方宣言
class EnemyManager;

class EnemySpawner {

public: //メンバ関数

	void Initialize(Vector3 position, EnemyManager* enemyManager);

	void Update();

	void Spawn();


private: //メンバ変数

	// 位置
	Vector3 position_;
	// 生成クールタイム
	float cooltime_;
	// 最大生成クールタイム
	const float kCooltime_ = 60 * 2;
	//エネミーマネージャー
	EnemyManager* enemyManager_;

};
