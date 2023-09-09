#pragma once
#include "Vector3.h"
#include "ColliderShape.h"
class OBB : public ColliderShape {

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~OBB() override {}

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 position, Vector3 rotate, Vector3 size);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Vector3 position, Vector3 rotate, Vector3 size) override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 角度指定した長さ
	/// </summary>
	Vector3 LengthOBB(Vector3 rotation) override;

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
	/// 中心点取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetCenter() override { return center_; }

	/// <summary>
	/// 座標軸取得
	/// </summary>
	/// <returns></returns>
	Vector3* GetOtientatuons() override { return otientatuons_; }

	/// <summary>
	/// 座標軸方向の長さの半分取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetSize() override { return size_; }
	
private: // メンバ変数

	Vector3 center_;          // 中心
	Vector3 otientatuons_[3]; // 座標軸
	Vector3 size_;            // 座標軸方向の長さの半分

};
