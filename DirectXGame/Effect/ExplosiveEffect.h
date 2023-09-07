#pragma once
#include "MyMath.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "BaseEffect.h"

/// <summary>
/// 破片エフェクト
/// </summary>
class ExplosiveEffect : public BaseEffect {
public: // メンバ関数
	
	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="models">破片モデル</param>
	/// <param name="translate">初期座標</param>
	/// <param name="strength">爆発強さ</param>
	void initialize(std::vector<Model*> models, Vector3 translate, float strength);

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
	const static int kMaxParticleCount_ = 10;

	// 粒子ごとのワールド座標
	WorldTransform worldTransformParticle_[kMaxParticleCount_];
	// 粒子ごとの終了フラグ
	bool isEndParticle_[kMaxParticleCount_];
	// 粒子ごとの速度
	Vector3 velocityParticle_[kMaxParticleCount_];
	// 粒子の落下速度
	float fallSpeed_[kMaxParticleCount_];

	// 粒子落下加速度
	float kFallAcceleration_;


};
