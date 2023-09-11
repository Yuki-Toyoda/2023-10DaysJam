#pragma once
#include "BaseEffect.h"
class EnemyDeathEffect : public BaseEffect {

public: // メンバ関数
	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="models">破片モデル</param>
	/// <param name="translate">テクスチャ</param>
	void initialize(std::vector<Model*> models, uint32_t textureHandle, Vector3 translate);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="viewProjection"><ビュープロジェクション/param>
	void Draw(const ViewProjection& viewProjection) override;

private: // メンバ変数

	// 粒子の数
	const static int kMaxParticleCount_ = 32;
	
	// 粒子ごとのワールド座標
	WorldTransform worldTransformParticle_[kMaxParticleCount_];
	// 粒子ごとの速度
	Vector3 velocityParticle_[kMaxParticleCount_];

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//タイマー
	uint32_t timer_;
	//タイム
	const uint32_t time_ = 20;

};
