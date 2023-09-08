#pragma once
#include "Vector3.h"
class Field {

public: // 静的メンバ関数
	    
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static Field* GetInstance();

public: //メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	 void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 最小値
	/// </summary>
	/// <returns></returns>
	Vector3 GetMin() { return min; }

	/// <summary>
	/// 最大値
	/// </summary>
	/// <returns></returns>
	Vector3 GetMax() { return max; }

private : // その他関数
	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables();

private: // メンバ関数
	Field() = default;
	~Field() = default;
	Field(const Field&) = delete;
	const Field& operator=(const Field&) = delete;

private: // メンバ変数

	//最小値
	Vector3 min;
	//最大値
	Vector3 max;

};
