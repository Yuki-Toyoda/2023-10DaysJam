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
		None, // 通常弾
		Fire, // 炎弾
		Ice, // 氷弾
		Thunder // 雷弾
	};

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

	/// <summary>
	/// 炎弾が衝突した状態セッター
	/// </summary>
	/// <param name="isHit">衝突したか</param>
	void SetIsHit(bool isHit) { isHit_ = isHit; }

private: // 弾ごとの行動関数

	/// <summary>
	/// 通常弾更新処理
	/// </summary>
	void NormalBulletUpdate();

	/// <summary>
	/// 炎弾更新処理
	/// </summary>
	void FireBulletUpdate();

	/// <summary>
	/// 水弾更新処理
	/// </summary>
	void WaterBulletUpdate();

	/// <summary>
	/// 雷弾更新処理
	/// </summary>
	void ThunderBulletUpdate();

private: //コライダー更新

	/// <summary>
	/// コライダー更新
	/// </summary>
	void ColliderUpdate();

public: // その他関数

	// 衝突時に呼ばれる関数
	void OnCollision(Collider* collision) override;

public: //自分取得

	virtual PlayerBullet* GetPlayerBullet() override { return this; }

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
	//反射係数
	float reflectionCoefficient;

	#pragma region 特殊弾用変数
	// 衝突判定
	bool isHit_;
	// 弾道落下用スピード
	float fallSpeed_;
	// 弾道落下スピード最大値
	float kMaxFallSpeed_;
	// 弾道落下用加速度
	float kFallAcceleration_;
	// 演出用t
	float animT_;
	// 演出用行動中間地点
	int actionWayPoint_;

#pragma endregion

	#pragma region 炎弾用変数
	// 爆破範囲
	Vector3 explosiveRange_;
	// 爆破演出時間
	float explosiveTime_;
#pragma endregion

	#pragma region 氷弾用変数
	// 展開する壁のサイズ
	Vector3 deployWallSize_;
	// 展開演出時間
	float deployWallStagingTime_;
	// 展開時間
	float deploymentWallTime_;
	// 終了演出時間
	float deployWallEndStagingTime_;
#pragma endregion

	#pragma region 雷弾用変数
	// 展開するダメージエリアのサイズ
	Vector3 deployAreaSize_;
	// 振動させる範囲
	float shakeRange_;
	// 展開演出時間
	float deployAreaStagingTime_;
	// 展開時間
	float deploymentAreaTime_;
	// 終了演出時間
	float deployAreaEndStagingTime_;
#pragma endregion


};
