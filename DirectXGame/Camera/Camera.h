#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"
#include "MyMath.h"

/// <summary>
/// カメラクラス
/// </summary>
class Camera {
public: // メンバ関数
	/// <summary>
	/// 初期化関数
	/// </summary>
	void Intialize();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update(bool isOption);

	/// <summary>
	/// セットアップ
	/// </summary>
	void SetUp();

public: // アクセッサ等

	/// <summary>
	/// ビュープロジェクションゲッター
	/// </summary>
	/// <returns>カメラのビュープロジェクション</returns>
	ViewProjection* GetViewProjection() { return &viewProjection_; }

	/// <summary>
	/// 追従対象セッター
	/// </summary>
	/// <param name="target">追従対象ワールド座標</param>
	void SetTarget(const WorldTransform* target) { target_ = target; }

	/// <summary>
	/// カメラシェイク強さのセッター
	/// </summary>
	/// <param name="shakeStrength">シェイク強さの変数ポインタ</param>
	void SetShakeStrength(const Vector2* shakeStrength) { shakeStrength_ = shakeStrength; }

	/// <summary>
	/// fovセッター
	/// </summary>
	/// <param name="fov"></param>
	void SetFov(float fov) { fov_ = fov; }

	/// <summary>
	/// fovゲッター
	/// </summary>
	/// <returns></returns>
	float GetFov() { return fov_; }

	/// <summary>
	/// カメラ感度セッター
	/// </summary>
	/// <param name="cameraSensitivity"></param>
	void SetCameraSensitivity(Vector2 cameraSensitivity) { cameraSensitivity_ = cameraSensitivity; }
	
	/// <summary>
	/// カメラ感度ゲッター
	/// </summary>
	/// <param name="cameraSensitivity"></param>
	Vector2 GetCameraSensitivity() { return cameraSensitivity_; }

private: // メンバ関数

	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables();

private: // メンバ変数

	// 入力検知用
	Input* input_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 追従対象のワールド座標
	const WorldTransform* target_ = nullptr;

	// カメラFOV
	float fov_;
	// カメラ感度
	Vector2 cameraSensitivity_;

	//Fov最大値
	const float fovMin_ = 0.45f;
	//Fov最小値
	const float fovMax_ = 1.2f;

	// cameraSensitivity最大値
	const float cameraSensitivityMax_ = 0.1f;
	// cameraSensitivity最小値
	const float cameraSensitivityMin_ = 0.001f;


	// カメラシェイク強さ
	const Vector2* shakeStrength_ = nullptr;

};
