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

	/// <summary>
	/// ジャンプ関数
	/// </summary>
	void Jump();

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

	// Aボタントリガー判定
	bool pressAButton_;
	// Bボタントリガー判定
	bool pressBButton_;
	// Xボタントリガー判定
	bool pressXButton_;
	// Yボタントリガー判定
	bool pressYButton_;

	// プレイヤー身長
	float height_;

	// 行動できるか
	bool canAction_;

	// 移動速度
	float moveSpeed_;
	// 最大落下ベクトル
	float kMaxFallSpeed_;
	// 現在落下速度
	float fallSpeed_;
	// 落下加速度
	float kFallAcceleration_;
	// 接地中か
	bool isGround_;

	// ジャンプできるか
	bool canJump_;
	// ジャンプ速度
	float jumpSpeed_;
	// 最大ジャンプ速度
	float kMaxJumpSpeed_;
	// ジャンプ減衰速度
	float kJumpDecayRate_;
};
