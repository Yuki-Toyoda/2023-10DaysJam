#pragma once
#include "Vector3.h"
#include "ColliderShape.h"
class Sphere : public ColliderShape {

public: //メンバ関数
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Sphere() override {}

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 posision, float radius);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="posision">ワールドポジション</param>
	void Update(Vector3 posision, float radius) override;

	/// <summary>
	/// 中心点取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetCenter() override { return center_; }

	/// <summary>
	/// 半径取得
	/// </summary>
	/// <returns></returns>
	float GetRadius() override { return radius_; }

private: // メンバ変数
	Vector3 center_; // 中心点
	float radius_;   // 半径

};
