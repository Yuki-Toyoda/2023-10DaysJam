#pragma once
#include <list>
#include <vector>
#include "../Charactor/BaseCharacter.h"
#include "Charactor/Player/PlayerBullet/PlayerBullet.h"
#include "ViewProjection.h"
#include "../input/Input.h"
#include "../audio/Audio.h"
#include "../math/MyMath.h"
#include "WinApp.h"
#include "Sprite.h"

/// <summary>
/// プレイヤークラス
/// </summary>
class Player : public BaseCharacter{
private: // サブクラス
	// UI構造体
	struct UI {
		uint32_t textureHandle_; // テクスチャ
		Vector2 position_; // 位置座標 
		Vector2 size_; // 大きさ
	};

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="modelsPlayer">プレイヤーモデル</param>
	/// <param name="modelsBullet">弾モデル</param>
	/// <param name="textureHandles">テクスチャ達</param>
	/// <param name="audioHandles">効果音達</param>
	void Initialize(
	    const std::vector<Model*>& modelsPlayer,
	    const std::vector<Model*>& modelsBullet, 
		const std::vector<uint32_t>& textureHandles,
		const std::vector<uint32_t>& audioHandles);

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

	/// <summary>
	/// セットアップ
	/// </summary>
	void Setup();

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
	/// バレットダメージゲッター
	/// </summary>
	uint32_t GetBulletDamage(PlayerBullet::BulletType bulletType) {
		return bulletDamage_[bulletType];
	}

	/// <summary>
	/// 接地状態ゲッター
	/// </summary>
	/// <returns>接地状態</returns>
	bool GetIsGround() { return isGround_; }

	/// <summary>
	/// ジャンプ可能状態のゲッター
	/// </summary>
	/// <returns>ジャンプ可能状態</returns>
	bool GetCanJump() { return canJump_; }

	/// <summary>
	/// 衝突時呼び出し関数
	/// </summary>
	void OnCollision(Collider* collision) override;

	/// <summary>
	/// オーブ追加関数
	/// </summary>
	/// <param name="orbType">追加するオーブタイプ</param>
	void AddOrbs(PlayerBullet::BulletType orbType);
	/// <summary>
	/// オーブ所持数確認
	/// </summary>
	/// <returns>オーブ所持数</returns>
	int GetHavingOrbCount() { return (int)havingOrbs_.size(); }

	/// <summary>
	/// オーブのリセット
	/// </summary>
	void ResetOrbs() {havingOrbs_.erase(havingOrbs_.begin(), havingOrbs_.end());}

	/// <summary>
	/// 特殊射撃強さゲッター
	/// </summary>
	/// <returns>特殊射撃強さ</returns>
	int32_t GetSpecialShotStrength() { return specialShotStrength_; }

	/// <summary>
	/// 選択されている変換タイプゲッター
	/// </summary>
	/// <returns>選択されている変換タイプ</returns>
	int GetSelectedType() { return selectedChangeType_; }
	/// <summary>
	/// 変換タイプセッター
	/// </summary>
	/// <param name="type">変換タイプ</param>
	void SetSelectedType(PlayerBullet::BulletType type) { selectedChangeType_ = (int)type; }

	/// <summary>
	/// 選択されているオーブのゲッター
	/// </summary>
	int32_t GetSelectedChangeOrb() { return selectedChangeOrb_; }
	/// <summary>
	/// 選択されているオーブセッター
	/// </summary>
	/// <param name="index">番号</param>
	void SetSelectedChangeOrb(int32_t index) { selectedChangeOrb_ = index; }

	/// <summary>
	/// カメラシェイク強さのゲッター
	/// </summary>
	/// <returns>カメラシェイク強さ変数のポインタ</returns>
	Vector2* GetShakeStrength() { return (Vector2*)&handOverCameraShakeStrength_; }

	/// <summary>
	/// 変換に必要な敵の数を減らす関数
	/// </summary>
	void SubtractNeedChangeOrbEnemyCount_();

	/// <summary>
	/// hp取得
	/// </summary>
	int32_t GetHp() { return hp_; }
	/// <summary>
	/// HPセッター
	/// </summary>
	/// <param name="hp">設定するHP</param>
	void SetHP(int32_t hp) { hp_ = hp; }

private: //衝突処理

	/// <summary>
	/// 氷との衝突処理
	/// </summary>
	void OnCollisionIce();

	/// <summary>
	/// エネミーとの衝突処理
	/// </summary>
	void OnCollisionEnemy();

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

	/// <summary>
	/// カメラシェイク再生開始関数(再生開始したいタイミングで一度だけ呼び出してください)
	/// </summary>
	/// <param name="shakeStrength">カメラシェイク強さ</param>
	/// <param name="shakeTime">振動秒数</param>
	void PlayCameraShake(Vector2 shakeStrength, float shakeTime);

	/// <summary>
	/// カメラシェイク関数(呼び出さなくていいです)
	/// </summary>
	void CameraShake();

	/// <summary>
	/// 位置制限
	/// </summary>
	void TranslationLimit();

private: // その他関数

	/// <summary>
	/// UIの更新処理
	/// </summary>
	void UIUpdate();

	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	void ApplyGlobalVariables() override;

public: // 自分取得
	virtual Player* GetPlayer() override { return this; }

private: // メンバ関数

	// 入力検知用
	Input* input_ = nullptr;
	// 音用
	Audio* audio_ = nullptr;

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
	// カメラに渡す変数
	Vector2 handOverCameraShakeStrength_;
	// 振動強さ
	Vector2 shakeStrength_;
	// カメラシェイク有効トリガー
	bool enableCameraShake_;
	// シェイク演出用t
	float shakeT_;
	// シェイク演出時間
	float shakeTime_;

	// テクスチャリスト
	std::vector<uint32_t> textureHandles_;

	// 効果音リスト
	std::vector<uint32_t> audioHandles_;

	// 照準スプライト
	std::unique_ptr<Sprite> spriteReticle_;
	// 3Dレティクル用座標
	WorldTransform worldTransform3DReticle_;

	// 十字ボタンUI
	UI spriteDpadUI_;
	std::unique_ptr<Sprite> spriteDpad_;
	// 十字ボタンUI
	std::unique_ptr<Sprite> spriteDpadUP_; // 上
	std::unique_ptr<Sprite> spriteDpadLeft_; // 左
	std::unique_ptr<Sprite> spriteDpadRight_; // 右

	// オーブ変換テキスト
	UI spriteChangeOrbUI_;
	std::unique_ptr<Sprite> spriteChangeOrbText_;

	// オーブ変換テキスト2
	UI spriteChangeOrbUI2_;
	std::unique_ptr<Sprite> spriteChangeOrbText2_;

	// オーブ変換に必要な敵数テキスト
	UI spriteNeedChangeOrbEnemyCountTextUI_;
	std::unique_ptr<Sprite> spriteNeedChangeOrbEnemyCountText_;

	// オーブ変換に必要な敵数スプライト
	UI spriteNeedChangeOrbEnemyCountUI_;
	std::unique_ptr<Sprite> spriteNeedChangeOrbEnemyCount_;

	// 右トリガースプライト
	UI spriteRightTriggerUI_;
	std::unique_ptr<Sprite> spriteRightTrigger_;
	// 通常射撃テキストスプライト
	UI spriteNormalShotTextUI_;
	std::unique_ptr<Sprite> spriteNormalShotText_;

	// 左トリガースプライト
	UI spriteLeftTriggerUI_;
	std::unique_ptr<Sprite> spriteLeftTrigger_;
	// 特殊射撃テキストスプライト
	UI spriteSpecialShotTextUI_;
	std::unique_ptr<Sprite> specialShotText_;
	// 現在の特殊射撃スプライト
	UI spriteNowSpecialShotPlanUI_;
	std::unique_ptr<Sprite> spriteSpecialShotPlan_;
	// 現在の特殊射撃倍率スプライト
	UI spriteSpecialShotMagnificationUI_;
	std::unique_ptr<Sprite> spriteSpecialShotMagnification_;

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
	// 接地サウンド再生トリガー
	bool isPlayLandSound_;

	// 足音の再生間隔
	int32_t playFootStepSELate_;
	// 足音の再生間隔
	int32_t kPlayFootStepSELate_ = 20;
	// 二つの効果音の内どちらを再生するか
	bool playFootStep2_;

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
	int selectedChangeType_;
	// 変換するオーブを示すスプライト
	std::unique_ptr<Sprite> spriteSelectedOrbs_;
	// 変換するオーブ
	int32_t selectedChangeOrb_;
	// 変換に必要な敵数
	int32_t needChangeOrbEnemyCount_;
	// 変換に必要な敵数のデフォルト値
	int32_t kNeedChangeOrbEnemyCount_;
	// 変換クールタイム
	int32_t changeCoolTime_;
	// 変換クールタイムデフォルト値
	int32_t kChangeCoolTime_;

	//バレットダメージ
	uint32_t bulletDamage_[4];

	//速度
	Vector3 velocity_ = {0.0f,0.0f,0.0f};
	// 前フレームの位置
	Vector3 preTranslation_ = {0.0f, 0.0f, 0.0f};
	// 氷の跳ね返り加速
	Vector3 accelerationIce_ = {0.0f, 0.0f, 0.0f};
	//氷の跳ね返り加速量ダウン
	Vector3 accelerationIceDown_ = {0.0f, 0.0f, 0.0f};

	//体力
	int32_t hp_; 
	//初期体力
	const uint32_t StartHp = 6;

	// 体力UI
	UI spriteHeartUI_;
	std::unique_ptr<Sprite> spriteHeart_[10];

	// 無敵か
	bool isInvincible_;
	// 無敵タイマー
	uint32_t invincibilityTimer_;
	// 衝突無敵タイマー
	uint32_t collisionInvincibilityTime_;

	#pragma region ImGuiテスト用変数
	#ifdef _DEBUG

	// 追加するオーブの種類
	int selectOrbs_;

	// シェイク強さ
	Vector2 testShakeStrength_;
	// シェイク秒数
	float testShakeTime_;

	#endif // _DEBUG
#pragma endregion


};
