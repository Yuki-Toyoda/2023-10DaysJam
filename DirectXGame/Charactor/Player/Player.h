#pragma once
#include <list>
#include <vector>
#include "../Charactor/BaseCharacter.h"
#include "Charactor/Player/PlayerBullet/PlayerBullet.h"
#include "ViewProjection.h"
#include "../input/Input.h"
#include "../math/MyMath.h"
#include "WinApp.h"
#include "Sprite.h"

/// <summary>
/// プレイヤークラス
/// </summary>
class Player : public BaseCharacter{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="modelsPlayer">プレイヤーモデル</param>
	/// <param name="modelsBullet">弾モデル</param>
	void Initialize(
	    const std::vector<Model*>& modelsPlayer,
	    const std::vector<Model*>& modelsBullet);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection) override;

	/// <summary>
	/// スプライト描画関数
	/// </summary>
	void SpriteDraw();

	/// <summary>
	/// コライダーの描画関数
	/// </summary>
	void ColliderDraw(bool enableDebugCamera);

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
	
	/// <summary>
	/// バレットゲッター
	/// </summary>
	/// <returns></returns>
	std::list<PlayerBullet*> GetBullets() { return bullets_; }

	/// <summary>
	/// 衝突時呼び出し関数
	/// </summary>
	void OnCollision() override;

	/// <summary>
	/// オーブ追加関数
	/// </summary>
	/// <param name="orbType">追加するオーブタイプ</param>
	void AddOrbs(PlayerBullet::BulletType orbType);

private: // 行動関数

	/// <summary>
	/// 移動関数
	/// </summary>
	void Move();

	/// <summary>
	/// ジャンプ関数
	/// </summary>
	void Jump();

	/// <summary>
	/// 射撃関数
	/// </summary>
	void Shot();
	/// <summary>
	/// リロード関数
	/// </summary>
	void Reload();

	/// <summary>
	/// 特殊射撃
	/// </summary>
	void SpecialShot();

private: // その他関数

	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables() override;

private: // メンバ関数

	// 入力検知用
	Input* input_ = nullptr;

	// Aボタントリガー判定
	bool pressAButton_;
	// Bボタントリガー判定
	bool pressBButton_;
	// Xボタントリガー判定
	bool pressXButton_;
	// Yボタントリガー判定
	bool pressYButton_;
	// 右トリガーのトリガー判定
	bool pressRTrigger_;
	// 左トリガーのトリガー判定
	bool pressLTrigger_;
	// 十字キーのトリガー判定
	bool pressDpadUp_; // 上
	bool pressDpadDown_; // 下
	bool pressDpadLeft_; // 左
	bool pressDpadRight_; // 右

	// トリガーデッドゾーン
	int32_t triggerDeadZone_R_; // 右
	int32_t triggerDeadZone_L_; // 左

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// 1x1テクスチャ
	uint32_t textureHandle1x1_ = 0u;
	// レティクル用テクスチャ
	uint32_t textureHandleReticle_ = 0u;

	// 照準スプライト
	std::unique_ptr<Sprite> spriteReticle_;
	// 3Dレティクル用座標
	WorldTransform worldTransform3DReticle_;

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
	// 最大ジャンプ高度
	float kMaxJumpHeight_;
	// ジャンプ減衰速度
	float kJumpDecayRate_;

	// プレイヤーの弾
	std::list<PlayerBullet*> bullets_;
	// プレイヤーの弾モデル
	std::vector<Model*> modelBullet_;
	// 射撃座標のオフセット
	Vector3 shotPosOffset_;
	// カメラから照準オブジェクトの距離
	float kDistanceToReticleObject_;
	// 射撃できるか
	bool canShot_;
	// 弾速
	float bulletSpeed_;
	// 射撃レートカウント用
	float fireCoolTime_;
	// 射撃レートデフォルト値
	float kMaxFireCoolTime_;
	// 現在弾数
	int32_t magazine_;
	// 最大弾数
	int32_t kMaxMagazine_;
	// リロード中か
	bool isReloading_;
	// 現在リロード時間
	int32_t reloadTime_;
	// リロード時間
	int32_t kMaxReloadTime_;

	// 所持しているオーブ
	std::vector<PlayerBullet::BulletType> havingOrbs_;
	// オーブ所持数最大値
	int32_t kMaxHavingOrbs_;
	// 所持しているオーブを描画するスプライト
	std::unique_ptr<Sprite> spriteHavingOrbs_[3];
	// スプライトの開始座標
	Vector2 spriteHavingOrbsStartPos_;
	// スプライトの大きさ
	Vector2 spriteHavingOrbsSize_;
	// スプライト１つの行間
	Vector2 spriteHavingOrbsLineSpace_;
	// 特殊射撃で撃つ予定の弾
	PlayerBullet::BulletType specialShotBulletPlans_;
	// 特殊射撃の強さ
	int32_t specialShotStrength_;
	// 特殊射撃できるか
	bool canSpecialShot_;
	// 変換するオーブの種類
	PlayerBullet::BulletType selectedChangeType_;
	// 変換するオーブ
	int32_t selectedChangeOrb_;
	// 変換クールタイム
	int32_t changeCoolTime_;
	// 変換クールタイムデフォルト値
	int32_t kChangeCoolTime_;

	#pragma region ImGuiテスト用変数
	#ifdef _DEBUG

	// 追加するオーブの種類
	int selectOrbs_;

	#endif // _DEBUG
#pragma endregion


};
