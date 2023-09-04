#pragma once 
#include "../Charactor/BaseCharacter.h"
class EnemyBullet : public BaseCharacter {

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデルデータ配列</param>
	/// <param name="models">位置</param>
	/// <param name="models">速度</param>
	void Initialize(
	    const std::vector<Model*>& models, const Vector3& position, const Vector3& velocity);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(const ViewProjection& viewProjection);

	// 衝突時に呼ばれる関数
	void OnCollision() override;

	// 死んでいるか
	bool IsDead() const { return isDead_; }

private: // メンバ変数
	// 速度
	Vector3 velocity_;

	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;


};
