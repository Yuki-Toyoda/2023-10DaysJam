#pragma once
#include "MyMath.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "BaseEffect.h"

/// <summary>
/// 火花エフェクト
/// </summary>
class SparkEffect : public BaseEffect {
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
	void initialize(
	    std::vector<Model*> models, Vector3 translate, float time, Vector2 occurrenceRange);

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

	// 同時再生する粒子の数
	const static int kMaxParticleCount_ = 10;
	const static int kMaxChildParticleCount_ = 6;

	// 粒子ごとのワールド座標
	WorldTransform worldTransformParticle_[kMaxParticleCount_];
	// 子粒子のワールド座標
	WorldTransform worldTransformChildParticle_[kMaxParticleCount_ * kMaxChildParticleCount_];
	// 粒子ごとの終了フラグ
	bool isEndParticle_[kMaxParticleCount_];
	// 子粒子ごとの終了フラグ
	bool isEndChildParticle_[kMaxParticleCount_ * kMaxChildParticleCount_];
	// 粒子ごとの速度
	Vector3 velocityParticle_[kMaxParticleCount_];
	// 子粒子ごとの速度
	Vector3 velocityChildParticle_[kMaxParticleCount_ * kMaxChildParticleCount_];
	// 粒子ごとの継続時間
	float particleActiveTime_[kMaxParticleCount_];
	// 粒子の落下速度
	float fallSpeed_[kMaxParticleCount_];
	// 粒子ごとの演出用t
	float particleT_[kMaxParticleCount_];
	// 粒子ごとの行動段階
	int actionWayPoint_[kMaxParticleCount_];

	// ループを抜けるフラグ
	bool roopBreak_;

	// 粒子落下加速度
	float kFallAcceleration_;
	// 演出用t
	float t_;
	// エフェクト継続時間
	float time_;
	// 粒子の発生範囲
	Vector2 occurrenceRange_;
	// 粒子発生間隔
	float occurrenceInterval_;
};
