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
	void Update();

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

	// カメラシェイク強さ
	const Vector2* shakeStrength_ = nullptr;

};
