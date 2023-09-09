#pragma once
#include "MyMath.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "BaseEffect.h"

/// <summary>
/// 雷エフェクト
/// </summary>
class SparkEffect: public BaseEffect{
private: // サブクラス

	// 行動中間地点
	enum ActionWayPoint {
		WayPoint1, // 行動中間地点1 ~
		WayPoint2,
		WayPoint3,
		WayPoint4,
		WayPoint5,
	};

public: // メンバ関数
	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="models">破片モデル</param>
	/// <param name="translate">初期座標</param>
	/// <param name="occurrenceRange">発生範囲</param>
	/// <param name="time">継続秒数</param>
	/// <param name="viewProjection">カメラのビュープロジェクションのポインタ</param>
	void initialize(
	    std::vector<Model*> models, Vector3 translate, float time, Vector2 occurrenceRange,
	    const ViewProjection* viewProjection);

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
	
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// 粒子の数
	const static int kMaxParticleCount_ = 10;

	// 粒子ごとのワールド座標
	WorldTransform worldTransformParticle_[kMaxParticleCount_];
	// 粒子ごとの終了フラグ
	bool isEndParticle_[kMaxParticleCount_];
	// 粒子ごとの最終座標
	Vector3 endTransformParticle_[kMaxParticleCount_];
	// 粒子ごとの最終サイズ
	Vector3 endSizeParticle_[kMaxParticleCount_];
	// 粒子演出用t
	float particleT_[kMaxParticleCount_];
	// 粒子ごとの演出時間
	float particleTime_[kMaxParticleCount_];
	// 粒子ごとの行動中間地点
	int actionWayPointParticle_[kMaxParticleCount_];
	// 演出用t
	float t_;
	// 継続秒数
	float time_;
	// 発生範囲
	Vector2 occurrenceRange_;
	// 発生間隔
	float occurrenceInterval_;
};
