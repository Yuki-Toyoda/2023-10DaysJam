#pragma once
#include "Vector3.h"
#include "ColliderShape.h"
class AABB : public ColliderShape {

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AABB() override {}

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 min, Vector3 max);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="min"></param>
	/// <param name="max"></param>
	void Update(Vector3 min, Vector3 max) override;

	/// <summary>
	/// グローバル変数に追加
	/// </summary>
	/// <param name="groupName"></param>
	void AddToGlobalVariables(const char* groupName) override;

	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables(const char* groupName) override;

	/// <summary>
	/// 最小点取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetMin() override { return min_; }

	/// <summary>
	/// 最大点取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetMax() override { return max_; }

private: // メンバ変数
	
	Vector3 min_; //最小点
	Vector3 max_; //最大点

};
