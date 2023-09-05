#pragma once
#include "../Charactor/BaseCharacter.h"
#include "../math/MyMath.h"

/// <summary>
/// プレイヤー弾クラス
/// </summary>
class PlayerBullet : public BaseCharacter {
public: // メンバ関数
	
	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="models">3Dモデル</param>
	/// <param name="startPos">初期座標</param>
	/// <param name="startotation">初期角度</param>
	/// <param name="velocity">弾速</param>
	void Initialize(const std::vector<Model*>& models, 
		Vector3 startPos, Vector3 startotation, Vector3 velocity);

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
	/// 死亡状態のゲッター
	/// </summary>
	/// <returns>死亡状態</returns>
	bool GetIsDead() { return isDead_; }
public: // その他関数

	// 衝突時に呼ばれる関数
	void OnCollision() override;

private: // メンバ変数

	// 死亡状態
	bool isDead_;
	// 弾の存在時間
	static const int32_t klifeTime_ = 60;
	// デスタイマー
	int32_t deathTimer_;

	// 弾速
	Vector3 velocity_;

};
