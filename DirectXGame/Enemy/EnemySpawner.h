#pragma once
#include "Vector3.h"
#include <stdint.h>

// エネミーマネージャー前方宣言
class EnemyManager;

class EnemySpawner {

public: //構造体
	struct PosChange {
		Vector3 position; // 生成位置情報
		uint32_t posChangeCooltime; // 最大生成位置変更タイム情報
	};

public: //メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="enemyManager"></param>
	void Initialize(EnemyManager* enemyManager);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// スポーン
	/// </summary>
	void Spawn();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

private: // その他関数

	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables();


private: //メンバ変数

	// 位置
	Vector3 position_;
	// 生成クールタイム
	uint32_t generateCooltime_;
	// 最大生成クールタイム
	const uint32_t kGenerateCooltime_ = 60 * 2;
	//エネミーマネージャー
	EnemyManager* enemyManager_;

	// 生成範囲
	Vector3 size_;
	
	// 生成位置変更タイム
	uint32_t posChangeCooltime_;

	// 移動回数
	static const uint32_t MoveNumberofTimes = 10;

	// 生成位置
	PosChange posChange[MoveNumberofTimes];

	// 移動回数情報
	uint32_t posChangeNum;


};
