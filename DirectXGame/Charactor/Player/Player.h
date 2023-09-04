#pragma once
#include "../Charactor/BaseCharacter.h"
#include "ViewProjection.h"
#include "../input/Input.h"
#include "../math/MyMath.h"

/// <summary>
/// プレイヤークラス
/// </summary>
class Player : public BaseCharacter{
public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデルデータ配列</param>
	void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection) override;

public: // アクセッサ等

	/// <summary>
	/// ワールド座標ゲッター
	/// </summary>
	/// <returns>ワールド座標</returns>
	WorldTransform* GetWorldTransform() { return &worldTransform_; }

	/// <summary>
	///	移動時のビュープロジェクションのセッター
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	// 衝突時に呼ばれる関数
	void OnCollision() override;

private: // 行動関数

	/// <summary>
	/// 移動関数
	/// </summary>
	void Move();

private: // その他関数
	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables() override;

private: // メンバ関数

	// 入力検知用
	Input* input_ = nullptr;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// 移動速度
	float moveSpeed_;

};
