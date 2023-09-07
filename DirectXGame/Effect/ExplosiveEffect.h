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
	/// <param name="models">モデル</param>
	/// <param name="translate">開始座標</param>
	void initialize(std::vector<Model*> models, Vector3 translate);

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
	const static int kMaxParticleCount_ = 5;

};
