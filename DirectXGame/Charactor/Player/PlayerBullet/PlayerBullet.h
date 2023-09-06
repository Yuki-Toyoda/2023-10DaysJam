#pragma once
#include "../Charactor/BaseCharacter.h"
#include "../math/MyMath.h"

/// <summary>
/// プレイヤー弾クラス
/// </summary>
class PlayerBullet : public BaseCharacter {
public: // サブクラス
	// 弾の種類
	enum BulletType {
		Normal,
		Fire,
		Water,
		Thunder
	};
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
	/// 初期化関数
	/// </summary>
	/// <param name="models">3Dモデル</param>
	/// <param name="startPos">初期座標</param>
	/// <param name="startotation">初期角度</param>
	/// <param name="velocity">弾速</param>
	/// <param name="bulletType">撃つ弾の種類</param>
	/// <param name="bulletStrength">撃つ弾の強さ</param>
	void Initialize(
	    const std::vector<Model*>& models, Vector3 startPos, Vector3 startotation, Vector3 velocity,
	    BulletType bulletType, int32_t bulletStrength);

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

	/// <summary>
	/// 弾の種類ゲッター
	/// </summary>
	/// <returns>弾の種類</returns>
	BulletType GetBulletType() { return bulletType_; }

private: // 弾ごとの行動関数

	/// <summary>
	/// 通常弾更新処理
	/// </summary>
	void NormalBulletUpdate();

	/// <summary>
	/// 爆発弾更新処理
	/// </summary>
	void FireBulletUpdate();

	/// <summary>
	///  ビーム弾更新処理
	/// </summary>
	void WaterBulletUpdate();

	/// <summary>
	/// 雷弾更新処理
	/// </summary>
	void ThunderBulletUpdate();

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

	// 発射する弾の種類
	BulletType bulletType_;
	// 発射する弾の強さ
	int32_t bulletStrength_;

};
