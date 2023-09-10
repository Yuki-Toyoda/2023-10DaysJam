#include "Player.h"
#include "../../config/GlobalVariables.h"
#include "TextureManager.h"
#include "Collision/ColliderShape/OBB.h"
#include <Ambient/Field.h>

void Player::Initialize(
    const std::vector<Model*>& modelsPlayer, const std::vector<Model*>& modelsBullet,
    const std::vector<uint32_t>& textureHandles) {

	// 基底クラス初期化
	BaseCharacter::Initialize(modelsPlayer);

	// 弾モデル読み込み
	modelBullet_ = modelsBullet;

	// テクスチャ受け取り
	textureHandles_ = textureHandles;

	// 入力情報取得
	input_ = Input::GetInstance();

	// 初期座標を設定
	worldTransform_.translation_ = {0.0f, 100.0f, 0.0f};

	// Aボタントリガー判定リセット
	pressAButton_ = false;
	// Bボタントリガー判定リセット
	pressBButton_ = false;
	// Xボタントリガー判定リセット
	pressXButton_ = false;
	// Yボタントリガー判定リセット
	pressYButton_ = false;
	// 右トリガーのトリガー判定
	pressRTrigger_ = false;
	// 左トリガーのトリガー判定
	pressLTrigger_ = false;

	// トリガーデッドゾーン設定
	triggerDeadZone_R_ = 25; // 右
	triggerDeadZone_L_ = 25; // 左

	// シェイク強さリセット
	handOverCameraShakeStrength_ = {0.0f, 0.0f};
	shakeStrength_ = {0.0f, 0.0f};
	// カメラシェイク無効
	enableCameraShake_ = false;
	// シェイク演出tリセット
	shakeT_ = 0.0f;
	// シェイク秒数リセット
	shakeTime_ = 0.0f;

	// 照準スプライト初期化
	spriteReticle_.reset(Sprite::Create(
	    textureHandles_[TextureManager::Reticle],
	    {(float)WinApp::kWindowWidth / 2, (float)WinApp::kWindowHeight / 2},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	// 照準スプライト大きさ設定
	spriteReticle_->SetSize({64.0f, 64.0f});
	// 3Dレティクルワールド座標の初期化
	worldTransform3DReticle_.Initialize();

	// 体力スプライト初期化
	spriteHeartUI_.textureHandle_ = textureHandles_[TextureManager::Heart];
	spriteHeartUI_.position_ = {50.0f, 50.0f};
	spriteHeartUI_.size_ = {64.0f, 64.0f};
	for (int i = 0; i < (int)StartHp; i++) {
		spriteHeart_[i].reset(Sprite::Create(
		    spriteHeartUI_.textureHandle_, 
			{spriteHeartUI_.position_.x + ((spriteHeartUI_.size_.x) * i), spriteHeartUI_.position_.y}, 
			{1.0f, 1.0f, 1.0f, 1.0f},
		    {0.5f, 0.5f}));
		spriteHeart_[i]->SetSize(spriteHeartUI_.size_);
	}

	// 十字ボタンUIの初期化
	spriteDpadUI_.textureHandle_ = textureHandles_[TextureManager::Dpad];
	spriteDpadUI_.position_ = {200.0f, 650.0f};
	spriteDpadUI_.size_ = {128.0f, 128.0f};
	spriteDpad_.reset(Sprite::Create(
	    spriteDpadUI_.textureHandle_, spriteDpadUI_.position_, {1.0f, 1.0f, 1.0f, 1.0f},
	    {0.5f, 0.5f}));
	// 十字ボタン矢印UI
	spriteDpadArrow_.reset(Sprite::Create(
	    textureHandles_[TextureManager::DpadArrow_P], spriteDpadUI_.position_, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	// 上ボタンUI
	spriteDpadUP_.reset(Sprite::Create(
	    textureHandles_[TextureManager::FireBullet], 
		{spriteDpadUI_.position_.x, spriteDpadUI_.position_.y - 96.0f},
	    {1.0f, 0.0f, 0.05f, 1.0f}, {0.5f, 0.5f}));
	spriteDpadUP_->SetSize({64.0f, 64.0f});
	// 左ボタンUI
	spriteDpadLeft_.reset(Sprite::Create(
	    textureHandles_[TextureManager::selectArrow_L],
	    {spriteDpadUI_.position_.x - 96.0f, spriteDpadUI_.position_.y},
	    {0.65f, 0.65f, 0.65f, 1.0f}, {0.5f, 0.5f}));
	spriteDpadLeft_->SetSize({64.0f, 64.0f});
	// 右ボタンUI
	spriteDpadRight_.reset(Sprite::Create(
	    textureHandles_[TextureManager::selectArrow_R],
	    {spriteDpadUI_.position_.x + 96.0f, spriteDpadUI_.position_.y},
	    {0.65f, 0.65f, 0.65f, 1.0f}, {0.5f, 0.5f}));
	spriteDpadRight_->SetSize({64.0f, 64.0f});

	// オーブ変換テキストの初期化
	spriteChangeOrbUI_.textureHandle_ = textureHandles_[TextureManager::RBHoldText]; // テクスチャ
	spriteChangeOrbUI_.position_ = {220.0f, 485.0f}; // 座標
	spriteChangeOrbUI_.size_ = {256.0f, 64.0f}; // 大きさ
	spriteChangeOrbText_.reset(Sprite::Create(
	    textureHandles_[TextureManager::RBHoldText],
	    {spriteChangeOrbUI_.position_.x, spriteChangeOrbUI_.position_.y},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	spriteChangeOrbText_->SetSize(spriteChangeOrbUI_.size_);

	// オーブ変換テキスト2のリセット
	spriteChangeOrbUI2_.textureHandle_ = textureHandles_[TextureManager::RBHoldText]; // テクスチャ
	spriteChangeOrbUI2_.position_ = {1125.0f, 125.0f};                                 // 座標
	spriteChangeOrbUI2_.size_ = {256.0f, 32.0f};                                      // 大きさ
	spriteChangeOrbText2_.reset(Sprite::Create(
	    textureHandles_[TextureManager::changeOrbText2],
	    {spriteChangeOrbUI2_.position_.x, spriteChangeOrbUI2_.position_.y},
	    {1.0f, 1.0f, 1.0f, 1.0f},
	    {0.5f, 0.5f}));
	spriteChangeOrbText2_->SetSize(spriteChangeOrbUI2_.size_);

	// オーブ変換に必要な敵数テキスト
	spriteNeedChangeOrbEnemyCountTextUI_.textureHandle_ =
	    textureHandles_[TextureManager::NeedEnemyCountText]; // テクスチャ
	spriteNeedChangeOrbEnemyCountTextUI_.position_ = {1050.0f, 175.0f}; 
	spriteNeedChangeOrbEnemyCountTextUI_.size_ = {384.0f, 64.0f}; 
	spriteNeedChangeOrbEnemyCountText_.reset(Sprite::Create(
	    spriteNeedChangeOrbEnemyCountTextUI_.textureHandle_,
	    spriteNeedChangeOrbEnemyCountTextUI_.position_,
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	spriteNeedChangeOrbEnemyCountText_->SetSize(spriteNeedChangeOrbEnemyCountTextUI_.size_);

	// オーブ変換に必要な敵数
	spriteNeedChangeOrbEnemyCountUI_.textureHandle_ =
	    textureHandles_[TextureManager::texture_0]; // テクスチャ
	spriteNeedChangeOrbEnemyCountUI_.position_ = {
	    spriteNeedChangeOrbEnemyCountTextUI_.position_.x +
	        (spriteNeedChangeOrbEnemyCountTextUI_.size_.x / 2.0f),
	    spriteNeedChangeOrbEnemyCountTextUI_.position_.y}; // 座標
	spriteNeedChangeOrbEnemyCountUI_.size_ = {64.0f, 64.0f}; // 大きさ
	spriteNeedChangeOrbEnemyCount_.reset(Sprite::Create(
	    spriteNeedChangeOrbEnemyCountUI_.textureHandle_, 
		spriteNeedChangeOrbEnemyCountUI_.position_,
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	spriteNeedChangeOrbEnemyCount_->SetSize(spriteNeedChangeOrbEnemyCountUI_.size_);

	// 右トリガースプライト
	spriteRightTriggerUI_.textureHandle_ =
	    textureHandles_[TextureManager::RT_N]; // テクスチャ
	spriteRightTriggerUI_.position_ = {1000.0f, 525.0f}; // 座標
	spriteRightTriggerUI_.size_ = {96.0f, 96.0f};       // 大きさ
	spriteRightTrigger_.reset(Sprite::Create(
	    spriteRightTriggerUI_.textureHandle_, spriteRightTriggerUI_.position_,
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	spriteRightTrigger_->SetSize(spriteRightTriggerUI_.size_);

	// 通常射撃テキストの初期化
	spriteNormalShotTextUI_.textureHandle_ = textureHandles_[TextureManager::NormalShotText];
	spriteNormalShotTextUI_.size_ = {256.0f, 64.0f}; // 大きさ
	spriteNormalShotTextUI_.position_ = {
	    spriteRightTriggerUI_.position_.x + (spriteRightTriggerUI_.size_.x) +
	        (spriteRightTriggerUI_.size_.x / 2.0f),
	    spriteRightTriggerUI_.position_.y}; // 座標
	spriteNormalShotText_.reset(Sprite::Create(
	    spriteNormalShotTextUI_.textureHandle_, spriteNormalShotTextUI_.position_,
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	spriteNormalShotText_->SetSize(spriteNormalShotTextUI_.size_);

	// 左トリガースプライト初期化
	spriteLeftTriggerUI_.textureHandle_ = textureHandles_[TextureManager::LT_N];
	spriteLeftTriggerUI_.position_ = {800.0f, 650.0f}; // 座標
	spriteLeftTriggerUI_.size_ = {96.0f, 96.0f};       // 大きさ
	spriteLeftTrigger_.reset(Sprite::Create(
	    spriteLeftTriggerUI_.textureHandle_, spriteLeftTriggerUI_.position_,
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	spriteLeftTrigger_->SetSize(spriteLeftTriggerUI_.size_);

	// 特殊射撃テキストの初期化
	spriteSpecialShotTextUI_.textureHandle_ = textureHandles_[TextureManager::SpecialShotText];
	spriteSpecialShotTextUI_.size_ = {256.0f, 64.0f};                                  // 大きさ
	spriteSpecialShotTextUI_.position_ = {
	    spriteLeftTriggerUI_.position_.x + (spriteSpecialShotTextUI_.size_.x / 2.0f) +
	        (spriteLeftTriggerUI_.size_.x / 2.0f),
	    spriteLeftTriggerUI_.position_.y}; // 座標
	specialShotText_.reset(Sprite::Create(
	    spriteSpecialShotTextUI_.textureHandle_, spriteSpecialShotTextUI_.position_,
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	specialShotText_->SetSize(spriteSpecialShotTextUI_.size_);

	// 現在の特殊射撃スプライトの初期化
	spriteNowSpecialShotPlanUI_.textureHandle_ = textureHandles_[TextureManager::None]; // テクスチャ
	spriteNowSpecialShotPlanUI_.position_ = {
	    spriteSpecialShotTextUI_.position_.x + (spriteSpecialShotTextUI_.size_.x / 1.5f),
	    spriteSpecialShotTextUI_.position_.y};          // 座標
	spriteNowSpecialShotPlanUI_.size_ = {96.0f, 96.0f}; // 大きさ
	spriteSpecialShotPlan_.reset(Sprite::Create(
	    spriteNowSpecialShotPlanUI_.textureHandle_,
	    spriteNowSpecialShotPlanUI_.position_,
	    {1.0f, 1.0f, 1.0f, 1.0f},
	    {0.5f, 0.5f}));
	spriteSpecialShotPlan_->SetSize(spriteNowSpecialShotPlanUI_.size_);

	// 現在の特殊射撃倍率スプライトの初期化
	spriteSpecialShotMagnificationUI_.textureHandle_ =
	    textureHandles_[TextureManager::Texturex1]; // テクスチャ
	spriteSpecialShotMagnificationUI_.position_ = {
	    spriteNowSpecialShotPlanUI_.position_.x + (spriteNowSpecialShotPlanUI_.size_.x / 1.25f),
	    spriteNowSpecialShotPlanUI_.position_.y + 20.0f};             // 座標
	spriteSpecialShotMagnificationUI_.size_ = {64.0f, 64.0f}; // 大きさ
	spriteSpecialShotMagnification_.reset(Sprite::Create(
	    spriteSpecialShotMagnificationUI_.textureHandle_, spriteSpecialShotMagnificationUI_.position_,
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	spriteSpecialShotMagnification_->SetSize(spriteSpecialShotMagnificationUI_.size_);

	// 身長高さ
	height_ = 20.0f;

	// 行動可能に
	canAction_ = true;

	// 移動速度初期化
	moveSpeed_ = 3.0f;
	// 最大落下速度設定
	kMaxFallSpeed_ = -9.8f;
	// 落下速度初期化
	fallSpeed_ = 0.0f;
	// 落下加速度
	kFallAcceleration_ = 0.098f;
	// 接地している
	isGround_ = false;

	// ジャンプ不可能に
	canJump_ = false;
	// ジャンプ速度初期化
	jumpSpeed_ = 0.0f;
	// 最大ジャンプ速度設定
	kMaxJumpHeight_ = 5.0f;
	// ジャンプ減衰速度を設定
	kJumpDecayRate_ = 0.098f;

	// 射撃座標のオフセット
	shotPosOffset_ = {0.0f, -5.0f, 0.0f};
	// カメラから照準オブジェクトの距離
	kDistanceToReticleObject_ = 150.0f;
	// 射撃可能に
	canShot_ = true;
	// 弾速設定
	bulletSpeed_ = 20.0f;
	// 発射レートリセット
	fireCoolTime_ = 0;
	// 発射レート設定
	kMaxFireCoolTime_ = 15;
	// 最大弾数設定
	kMaxMagazine_ = 15;
	// 弾数リセット
	magazine_ = kMaxMagazine_;
	// リロード中ではない
	isReloading_ = false;
	// リロード時間設定
	kMaxReloadTime_ = 60;
	// リロード時間リセット
	reloadTime_ = kMaxReloadTime_;

	// オーブ所持数最大値設定
	kMaxHavingOrbs_ = 3;
	// 所持オーブのリセット
	havingOrbs_.erase(havingOrbs_.begin(), havingOrbs_.end());
	// 開始座標設定
	spriteHavingOrbsStartPos_ = {1000.0f, 100.0f};
	// 大きさ設定
	spriteHavingOrbsSize_ = {64.0f, 64.0f};
	// 行間設定
	spriteHavingOrbsLineSpace_ = {96.0f, 0.0f};
	for (int i = 0; i < 3; i++) {
		// スプライトの生成
		spriteHavingOrbs_[i].reset(Sprite::Create(
		    textureHandles_[TextureManager::Orb],
		    {spriteHavingOrbsStartPos_.x + (spriteHavingOrbsLineSpace_.x * i),
		     spriteHavingOrbsStartPos_.y + (spriteHavingOrbsLineSpace_.y * i)},
		    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
		// サイズ設定
		spriteHavingOrbs_[i]->SetSize(spriteHavingOrbsSize_);
	}
	// 変換するオーブを示すスプライトのリセット
	spriteSelectedOrbs_.reset(Sprite::Create(
	    textureHandles_[TextureManager::selectOrb],
	    {spriteHavingOrbsStartPos_.x,
	     spriteHavingOrbsStartPos_.y},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	// スプライトのサイズ設定
	spriteSelectedOrbs_->SetSize({80.0f, 80.0f});
	// 特殊射撃で撃つ予定の弾のリセット
	specialShotBulletPlans_ = PlayerBullet::None;
	// 特殊射撃の強さをリセット
	specialShotStrength_ = 1;
	// 特殊射撃できるか
	canSpecialShot_ = false;
	// チャージするオーブの種類リセット
	selectedChangeType_ = PlayerBullet::Fire;
	// 現在選択しているオーブ
	selectedChangeOrb_ = 0;
	// 変換に必要な敵数のデフォルト値設定
	kNeedChangeOrbEnemyCount_ = 5;
	// 変換に必要な敵数設定
	needChangeOrbEnemyCount_ = kNeedChangeOrbEnemyCount_;
	// 変換クールタイム
	changeCoolTime_ = 0;
	// 変換クールタイムデフォルト値
	kChangeCoolTime_ = 120;

	// バレットダメージ
	bulletDamage_[0] = 1;
	bulletDamage_[1] = 1;
	bulletDamage_[2] = 1;
	bulletDamage_[3] = 1;

	// 体力
	hp = StartHp; 

	// 無敵か
	isInvincible_ = false;
	// 無敵タイマー
	invincibilityTimer_ = 0;
	// 衝突無敵タイマー
	collisionInvincibilityTime_ = 20;

#pragma region ImGuiテスト用変数
#ifdef _DEBUG

	// 追加するオーブの種類リセット
	selectOrbs_ = PlayerBullet::Fire;

	// シェイク強さリセット
	testShakeStrength_ = {0.0f, 0.0f};
	// シェイク秒数リセット
	testShakeTime_ = 0.0f;

#endif // _DEBUG
#pragma endregion

	// 衝突属性を設定
	SetCollisionAttribute(0xfffffffe);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(0x00000001);
	//タグ
	tag_ = TagPlayer;

	// コライダーの形
	OBB* obb = new OBB();
	obb->Initialize(GetWorldPosition(), worldTransform_.translation_, Vector3(3.0f,3.0f,3.0f));
	colliderShape_ = obb;
	
	#pragma region 調整項目クラス
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "Player";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加
	globalVariables->AddItem(groupName, "Height", height_);              // 身長
	globalVariables->AddItem(groupName, "MoveSpeed", moveSpeed_);        // 移動速度
	globalVariables->AddItem(groupName, "MaxFallSpeed", kMaxFallSpeed_); // 最大落下速度
	globalVariables->AddItem(groupName, "FallAcceleration", kFallAcceleration_); // 落下加速度
	globalVariables->AddItem(groupName, "MaxJumpHeight", kMaxJumpHeight_); // 最大ジャンプ高度
	globalVariables->AddItem(groupName, "JumpDecayRate", kJumpDecayRate_); // ジャンプ減衰速度
	globalVariables->AddItem(groupName, "ShotPosOffset", shotPosOffset_); // 射撃座標オフセット
	globalVariables->AddItem(
	    groupName, "DistanceToReticleObject",
	    kDistanceToReticleObject_); // カメラから照準オブジェクトの距離
	globalVariables->AddItem(groupName, "BulletSpeed", bulletSpeed_);      // 弾速
	globalVariables->AddItem(groupName, "MaxMagazineSize", kMaxMagazine_); // 最大弾数設定
	globalVariables->AddItem(groupName, "MaxReloadTime", kMaxReloadTime_); // リロードにかかる時間
	globalVariables->AddItem(
	    groupName, "SpriteHavingOrbsStartPos",
	    spriteHavingOrbsStartPos_); // 所持オーブ描画UIの開始座標
	globalVariables->AddItem(
	    groupName, "SpriteHavingOrbsSize", spriteHavingOrbsSize_); // 所持オーブ描画UIの大きさ
	globalVariables->AddItem(
	    groupName, "SpriteHavingOrbsLineSpace",
	    spriteHavingOrbsLineSpace_); // 所持オーブ描画UIの行間

	// バレットダメージ
	globalVariables->AddItem(
	    groupName, "BulletDamageNone", int(bulletDamage_[PlayerBullet::BulletType::None]));
	globalVariables->AddItem(
	    groupName, "BulletDamageFire", int(bulletDamage_[PlayerBullet::BulletType::Fire]));
	globalVariables->AddItem(
	    groupName, "BulletDamageIce", int(bulletDamage_[PlayerBullet::BulletType::Ice]));
	globalVariables->AddItem(
	    groupName, "BulletDamageThunder", int(bulletDamage_[PlayerBullet::BulletType::Thunder]));

	globalVariables->AddItem(
	    groupName, "CollisionInvincibilityTime", int(collisionInvincibilityTime_));

	colliderShape_->AddToGlobalVariables(groupName);
#pragma endregion
}

void Player::Update() {

	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->GetIsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//前フレームの位置を保存
	preTranslation_ = worldTransform_.translation_;

	// 無敵タイマー処理
	if (isInvincible_) {
		if (--invincibilityTimer_ == 0) {
			isInvincible_ = false;
		}
	}

	// 行動可能なら
	if (canAction_) {
		// 移動処理
		Move();
		// ジャンプ処理
		Jump();
		// 射撃
		Shot();
		// リロード
		Reload();
		// 特殊射撃
		SpecialShot();
	}

	// カメラシェイクが有効ならカメラシェイクさせる
	if (enableCameraShake_) {
		CameraShake();
	}

	// UIの更新処理
	UIUpdate();
	// 調整項目を反映
	ApplyGlobalVariables();

	// 位置制限
	TranslationLimit();
	// 基底クラス更新
	BaseCharacter::Update();

	// コライダー更新
	colliderShape_->Update(GetWorldPosition(), worldTransform_.rotation_,colliderShape_->GetSize());

	// 3Dレティクルのワールド座標更新
	worldTransform3DReticle_.UpdateMatrix();

	// 弾の更新処理
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// ボタンのトリガー判定をとる
	// ゲームパッドの状態取得
	XINPUT_STATE joyState;
	if (input_->GetJoystickState(0, joyState)) {
		// Aボタンが押されていなければ
		if (!(joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A))
			pressAButton_ = false;
		else
			pressAButton_ = true;
		// Bボタンが押されていなければ
		if (!(joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B))
			pressBButton_ = false;
		else
			pressBButton_ = true;
		// Xボタンが押されていなければ
		if (!(joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X))
			pressXButton_ = false;
		else
			pressXButton_ = true;
		// Yボタンが押されていなければ
		if (!(joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y))
			pressYButton_ = false;
		else
			pressYButton_ = true;
		// 右トリガーが押されていなければ
		if (!(joyState.Gamepad.bRightTrigger > triggerDeadZone_R_))
			pressRTrigger_ = false;
		else
			pressRTrigger_ = true;
		// 左トリガーが押されていなければ
		if (!(joyState.Gamepad.bLeftTrigger > triggerDeadZone_L_))
			pressLTrigger_ = false;
		else
			pressLTrigger_ = true;

		// 十字キー上が押されていなければ
		if (!(joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP))
			pressDpadUp_ = false;
		else
			pressDpadUp_ = true;
		// 十字キー下が押されていなければ
		if (!(joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN))
			pressDpadDown_ = false;
		else
			pressDpadDown_ = true;
		// 十字キー左が押されていなければ
		if (!(joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT))
			pressDpadLeft_ = false;
		else
			pressDpadLeft_ = true;
		// 十字キー右が押されていなければ
		if (!(joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT))
			pressDpadRight_ = false;
		else
			pressDpadRight_ = true;
	}

	#ifdef _DEBUG

	ImGui::Begin("player");

	// 追加するオーブを選択
	ImGui::RadioButton("Fire", &selectOrbs_, PlayerBullet::Fire);
	ImGui::SameLine();
	ImGui::RadioButton("Water", &selectOrbs_, PlayerBullet::Ice);
	ImGui::SameLine();
	ImGui::RadioButton("Thunder", &selectOrbs_, PlayerBullet::Thunder);

	if (ImGui::Button("AddOrbs")) {
		// 選択したオーブを追加
		AddOrbs((PlayerBullet::BulletType)selectOrbs_);
	}

	if (ImGui::Button("Reset")) {
		// 所持オーブのリセット
		havingOrbs_.erase(havingOrbs_.begin(), havingOrbs_.end());
	}

	if (ImGui::TreeNode("HavingOrbs")) {
		for (int i = 0; i < havingOrbs_.size(); i++) {
			int OrbType = havingOrbs_[i];
			ImGui::DragInt("havingOrbType", &OrbType, 1.0f);
		}
		ImGui::TreePop();
	}

	int specialShotPlan = specialShotBulletPlans_;
	int selectedType = selectedChangeType_;
	ImGui::DragInt("SpecialShotPlan", &specialShotPlan, 1.0f);
	ImGui::DragInt("SpecialShotStrength", &specialShotStrength_, 1.0f);
	ImGui::DragInt("selectedType", &selectedType, 1.0f);
	ImGui::DragInt("selectedOrb", &selectedChangeOrb_, 1.0f);
	ImGui::DragInt("ChangeCoolTime", &changeCoolTime_, 1.0f);
	ImGui::DragInt("DefaultChangeCoolTime", &kChangeCoolTime_, 1.0f);

	ImGui::DragFloat2("shakeStrength", &testShakeStrength_.x, 0.1f);
	ImGui::DragFloat("shakeTime", &testShakeTime_, 0.1f);
	if (ImGui::Button("cameraShake")) {
		PlayCameraShake(testShakeStrength_, testShakeTime_);
	}
	ImGui::DragFloat3("wT", &worldTransform_.translation_.x, 1.0f);
	ImGui::DragInt("HP", &hp);

	ImGui::DragInt("needChangeOrbEnemyCount_", &needChangeOrbEnemyCount_, 1.0f);
	if (ImGui::Button("SubtractNeedChangeOrbEnemyCount")) {
		SubtractNeedChangeOrbEnemyCount_();
	}

	ImGui::End();

#endif // _DEBUG


}

void Player::Draw(const ViewProjection& viewProjection) { 
	// 弾の更新処理
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::SpriteDraw() {
	// 照準描画
	spriteReticle_->Draw();

	// ゲームパッドの状態取得
	XINPUT_STATE joyState;
	if (input_->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			
		}
	}

	// 十字ボタンUI描画
	spriteDpad_->Draw();
	spriteDpadArrow_->Draw();      // 十字ボタン選択矢印描画
	spriteDpadUP_->Draw();         // 十字上ボタンUI描画
	spriteDpadLeft_->Draw();       // 十字左ボタンUI描画
	spriteDpadRight_->Draw();      // 十字右ボタンUI描画
	spriteChangeOrbText2_->Draw(); // オーブ変換テキスト2

	// 変換に必要な敵数
	spriteNeedChangeOrbEnemyCountText_->Draw();
	spriteNeedChangeOrbEnemyCount_->Draw();

	// 通常射撃描画
	spriteRightTrigger_->Draw();
	spriteNormalShotText_->Draw();

	// 現在の特殊射撃を描画
	spriteLeftTrigger_->Draw();
	specialShotText_->Draw();
	spriteSpecialShotPlan_->Draw();
	spriteSpecialShotMagnification_->Draw();

	// 現在の体力描画
	for (int i = 0; i < hp; i++) {
		spriteHeart_[i]->Draw();
	}

	// オーブ変換テキストUIの描画
	spriteChangeOrbText_->Draw();

	// 所持オーブが1個でもあったら
	if ((int)havingOrbs_.size() > 0) {
		// 変換するオーブを示すスプライト表示
		spriteSelectedOrbs_->Draw();
	}

	// 所持オーブの描画
	for (int i = 0; i < 3; i++)
		spriteHavingOrbs_[i]->Draw();

}

void Player::ColliderDraw(bool enableDebugCamera) {

	if (enableDebugCamera) {
		GetColliderShape()->Draw();
	}

	for (PlayerBullet* bullet : bullets_) {
		bullet->GetColliderShape()->Draw();
	}
}

void Player::OnCollision(Collider* collision) { 
	
	if (collision->GetTag() == TagPlayerBulletIce) {
		OnCollisionIce();
	} else if (collision->GetTag() == TagEnemy || 
		collision->GetTag() == TagBossEnemy ||
	    collision->GetTag() == TagEnemyBullet) {
		if (!isInvincible_) {
			OnCollisionEnemy();
		}
	}

}

void Player::AddOrbs(PlayerBullet::BulletType orbType) {
	// 所持しているオーブ数を確認
	int havingOrbCount = (int)havingOrbs_.size();
	// 所持しているオーブの数が３つ以上の時
	if (havingOrbCount >= kMaxHavingOrbs_) {
		// 所持している最初のオーブを削除
		havingOrbs_.erase(havingOrbs_.begin());
	}
	// 引数のオーブを追加
	havingOrbs_.push_back(orbType);
}

void Player::SubtractNeedChangeOrbEnemyCount_() {
	// 変換に必要な敵数が0以下ではない時
	if (needChangeOrbEnemyCount_ > 0) {
		// 敵数デクリメント
		needChangeOrbEnemyCount_--;
	} else {
		// 敵数固定
		needChangeOrbEnemyCount_ = 0;
	}
}

void Player::OnCollisionIce() {

	float reflection = -10.0f;
	float accelerationDown = 1.0f / 2.0f;

	worldTransform_.translation_ = preTranslation_;
	accelerationIce_ = MyMath::Normalize(velocity_) * reflection;
	accelerationIceDown_ = MyMath::Normalize(velocity_) * accelerationDown;
	worldTransform_.UpdateMatrix();
	
}

void Player::OnCollisionEnemy() {

	//ダメージ受ける
	hp--;
	invincibilityTimer_ = collisionInvincibilityTime_;
	isInvincible_ = true;

	//カメラシェイク
	Vector2 shakeStrength = { 5.0f,5.0f}; // カメラシェイク強さ
	float shakeTime = 1.0f; // シェイクタイム
	PlayCameraShake(shakeStrength, shakeTime);

}

void Player::Move() {
	// ゲームパッドの状態取得
	XINPUT_STATE joyState;
	if (input_->GetJoystickState(0, joyState)) {
		// スティックの入力に応じて移動
		Vector3 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX};

		// 移動量を正規化、スピードを加算
		move = MyMath::Normalize(move) * moveSpeed_;

		// カメラの角度から回転行列を生成
		Matrix4x4 rotateMat = MyMath::MakeRotateYMatrix(viewProjection_->rotation_.y);
		// 移動ベクトルをカメラの角度に応じて回転させる
		move = MyMath::Transform(move, rotateMat);

		//氷の跳ね返り
		velocity_ = move + accelerationIce_;
		if (accelerationIce_.x != 0.0f) {
			Vector3 preAccel = accelerationIce_;
			accelerationIce_ = accelerationIce_ + accelerationIceDown_;
			if (accelerationIce_.x * preAccel.x < 0.0f || accelerationIce_.y * preAccel.y < 0.0f ||
			    accelerationIce_.z * preAccel.z < 0.0f) {
				accelerationIce_.x = 0.0f;
				accelerationIce_.y = 0.0f;
				accelerationIce_.z = 0.0f;
			}
		}

		// 移動
		worldTransform_.translation_ = worldTransform_.translation_ + velocity_;
		// プレイヤーの向きを移動方向に合わせる
		worldTransform_.rotation_ = viewProjection_->rotation_;

	}

	// 接地していないなら
	if (!isGround_) {
		// 最大落下速度を超過するまで
		if (fallSpeed_ >= kMaxFallSpeed_) {
			// 落下速度加算
			fallSpeed_ -= kFallAcceleration_;
		} else {
			// 超過していた場合は落下速度を最大速度に設定
			fallSpeed_ = kMaxFallSpeed_;
		}
	} else {
		// 接地しているなら落下速度初期化
		fallSpeed_ = 0.0f;
	}

	// １フレーム落下した後の座標を一時的に計算
	float tempTranslateY = worldTransform_.translation_.y + fallSpeed_;
	// 一時座標がプレイヤー身長以下になったら
	if (tempTranslateY <= height_) {
		// 座標を床位置にリセット
		worldTransform_.translation_.y = height_;
		// 接地判定をTrue
		isGround_ = true;
		// 落下速度初期化
		fallSpeed_ = 0.0f;
	} else {
		// 落下スピード加算
		worldTransform_.translation_.y += fallSpeed_;
	}

}

void Player::Jump() { 
	// ジャンプ可能なら
	if (canJump_) {
		// ゲームパッドの状態取得
		XINPUT_STATE joyState;
		if (input_->GetJoystickState(0, joyState)) {
			// Aボタンが押されたら
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A && !pressAButton_) {
				// ジャンプさせる
				jumpSpeed_ = kMaxJumpHeight_;
				// ジャンプ不可に
				canJump_ = false;
				// 接地していない状態に
				isGround_ = false;
			}
		}
	} else {
		// 接地しているなら
		if (isGround_) {
			// ジャンプ可能に
			canJump_ = true;
		}
	}

	// 接地していないなら
	if (!isGround_) {
		// ジャンプ処理
		worldTransform_.translation_.y += jumpSpeed_;
	}

	// ジャンプ速度がでないなら
	if (jumpSpeed_ >= 0.0f) {
		// ジャンプ速度減衰
		jumpSpeed_ -= kJumpDecayRate_;
	} else {
		// ジャンプ速度を0に
		jumpSpeed_ = 0.0f;
	}

}

void Player::Shot() {

	// ビューポート行列の生成
	Matrix4x4 matViewport =
	    MyMath::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成
	Matrix4x4 matViewProjectionViewport = viewProjection_->matView * viewProjection_->matProjection * matViewport;
	// その逆行列を作る
	Matrix4x4 matInverseVPV = MyMath::Inverse(matViewProjectionViewport);

	// スクリーン座標
	Vector3 posNear =
	    Vector3((float)(WinApp::kWindowWidth / 2), (float)(WinApp::kWindowHeight / 2), 0);
	Vector3 posFar =
	    Vector3((float)(WinApp::kWindowWidth / 2), (float)(WinApp::kWindowHeight / 2), 1);

	// スクリーンからワールド座標系に
	posNear = MyMath::Transform(posNear, matInverseVPV);
	posFar = MyMath::Transform(posFar, matInverseVPV);

	// マウスレイの方向
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = MyMath::Normalize(mouseDirection);

	// カメラから照準オブジェクトの距離を設定
	mouseDirection = mouseDirection * kDistanceToReticleObject_;

	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = mouseDirection + posNear;

	// ゲームパッドの状態取得
	XINPUT_STATE joyState;
	if (input_->GetJoystickState(0, joyState)) {
		// Rトリガーが押されたら
		if (joyState.Gamepad.bRightTrigger > triggerDeadZone_R_) {
			// 射撃可能、リロード中ではなく、弾数が1発以上なら
			if (fireCoolTime_ <= 0 && !isReloading_ && magazine_ > 0) {
				// 弾の生成
				PlayerBullet* newBullet = new PlayerBullet();

				// 射撃ベクトル計算用
				Vector3 shotVelocity;

				// 移動ベクトルを初期化する
				shotVelocity = {0.0f, 0.0f, 0.0f};

				// レティクルのワールド座標を求める
				Vector3 ReticleWorldPos;
				ReticleWorldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
				ReticleWorldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
				ReticleWorldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

				// 射撃座標調整
				Vector3 shotPos = worldTransform_.translation_;
				shotPos = shotPos + shotPosOffset_;
				shotVelocity = ReticleWorldPos - shotPos;
				shotVelocity = MyMath::Normalize(shotVelocity) * bulletSpeed_;

				// 生成した弾を初期化
				newBullet->Initialize(
				    modelBullet_,  // 3Dモデル
				    shotPos,                    // 初期位置
					viewProjection_->rotation_, // 初期角度
				    shotVelocity); // 弾速

				// 生成した弾をリストに入れる
				bullets_.push_back(newBullet);

				// 弾数デクリメント
				//magazine_--;

				// 射撃クールタイムリセット
				fireCoolTime_ = kMaxFireCoolTime_;
			}
		}
	}

	if (fireCoolTime_ > 0) {
		// 射撃クールタイムデクリメント
		fireCoolTime_--;
	} else {
		// 射撃クールタイム固定
		fireCoolTime_ = 0;
	}

}

void Player::Reload() {
	// ゲームパッドの状態取得
	XINPUT_STATE joyState;
	if (input_->GetJoystickState(0, joyState)) {
		// xボタンが押されたらリロード待機状態に
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X && !pressXButton_) {
			// リロード中ではなく、弾数が減っているなら
			if (!isReloading_ && magazine_ < kMaxMagazine_) {
				// リロード中状態に
				isReloading_ = true;
			}
		}
	}

	// マガジンが空なら
	if (magazine_ == 0) {
		// 強制的にリロード
		isReloading_ = true;
	}

	// リロード中かつ射撃可能状態なら
	if (isReloading_ && fireCoolTime_ <= 0) {
		if (reloadTime_ >= 0) {
			// リロードカウントデクリメント
			reloadTime_--;
		} else {
			// 弾数リセット
			magazine_ = kMaxMagazine_;
			// リロード時間リセット
			reloadTime_ = kMaxReloadTime_;
			// リロード中ではない
			isReloading_ = false;
		}
	}
		
}

void Player::SpecialShot() {

	// 特殊射撃の強さをリセット
	specialShotStrength_ = 1;
	// 特殊射撃の予定をリセット
	specialShotBulletPlans_ = PlayerBullet::None;

	// 所持しているオーブ数を確認
	int havingOrbCount = (int)havingOrbs_.size();
	for (int i = 0; i < havingOrbCount; i++) {
		// 該当するオーブの色を取得
		PlayerBullet::BulletType haveOrb = havingOrbs_[i];

		// 特殊射撃がもともと同じだったら
		if (specialShotBulletPlans_ == haveOrb) {
			// 特殊射撃の強さをインクリメント
			specialShotStrength_++;
		}
		// 特殊射撃の強さが1以下なら
		else if (specialShotStrength_ <= 1) {
			// 特殊射撃の予定変更
			specialShotBulletPlans_ = haveOrb;
			// 強さリセット
			specialShotStrength_ = 1;
		}

	}
	if (specialShotStrength_ <= 1 && havingOrbCount > 0) {
		// 最初のオーブの特殊射撃を使用
		specialShotBulletPlans_ = havingOrbs_[0];
		// 強さリセット
		specialShotStrength_ = 1;
	}

	// オーブを所持していない場合、特殊射撃は行えない
	if (specialShotBulletPlans_ == PlayerBullet::None)
		canSpecialShot_ = false;
	else
		canSpecialShot_ = true;

	// ゲームパッドの状態取得
	XINPUT_STATE joyState;
	if (input_->GetJoystickState(0, joyState)) {
		// 左トリガーが押されたら特殊射撃
		if (joyState.Gamepad.bLeftTrigger > triggerDeadZone_L_ && !pressLTrigger_) {
			if (canSpecialShot_) {
				// 弾の生成
				PlayerBullet* newBullet = new PlayerBullet();

				// 射撃ベクトル計算用
				Vector3 shotVelocity;

				// 移動ベクトルを初期化する
				shotVelocity = {0.0f, 0.0f, 0.0f};

				// レティクルのワールド座標を求める
				Vector3 ReticleWorldPos;
				ReticleWorldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
				ReticleWorldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
				ReticleWorldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

				// 射撃座標調整
				Vector3 shotPos = worldTransform_.translation_;
				shotPos = shotPos + shotPosOffset_;
				shotVelocity = ReticleWorldPos - shotPos;
				shotVelocity = MyMath::Normalize(shotVelocity) * 7.5f;
				// 壁用テクスチャ
				std::vector<uint32_t> wallTextureHandles = {
				    textureHandles_[TextureManager::IceWall], // 氷壁テクスチャ
				    textureHandles_[TextureManager::IceWallDamage1], // 氷壁ダメージ1段階目
				    textureHandles_[TextureManager::IceWallDamage2], // 氷壁ダメージ2段階目
				    
				};
				// 生成した弾を初期化
				newBullet->Initialize(
				    modelBullet_,               // 3Dモデル
				    wallTextureHandles,			// 壁用テクスチャリスト
				    shotPos,                    // 初期位置
				    viewProjection_->rotation_, // 初期角度
				    shotVelocity,               // 弾速
					specialShotBulletPlans_,	// 行う特殊射撃
					specialShotStrength_		// 特殊攻撃の強さ
					);             

				// 生成した弾をリストに入れる
				bullets_.push_back(newBullet);

				// 所持オーブのリセット
				havingOrbs_.erase(havingOrbs_.begin(), havingOrbs_.end());

				// 特殊射撃の予定をリセット
				specialShotBulletPlans_ = PlayerBullet::None;

			}
		}

		// 変換するオーブの種類を選択
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP && !pressDpadUp_) {
			if (selectedChangeType_ < PlayerBullet::Thunder) {
				// 変換タイプを次のに設定
				selectedChangeType_++;
			} else {
				// 変換タイプをリセット
				selectedChangeType_ = PlayerBullet::Fire;
			}
		}

		// 十字右
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT && !pressDpadRight_) {
			// 選択しているオーブが最大値を上回っていたら
			if (selectedChangeOrb_ >= havingOrbCount - 1) {
				// 最初に戻る
				selectedChangeOrb_ = 0;
			} else {
				selectedChangeOrb_++;
			}
		}
		// 十字左
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT && !pressDpadLeft_) {
			// 選択しているオーブがをした回っていたら
			if (selectedChangeOrb_ <= 0) {
				// 最初に戻る
				if (havingOrbCount > 0) {
					selectedChangeOrb_ = havingOrbCount - 1;
				}
			} else {
				selectedChangeOrb_--;
			}
		}

		// オーブの数が1以上なら
		if (havingOrbCount > 0 && needChangeOrbEnemyCount_ <= 0) {
			// 選択したオーブを別のオーブに変換する
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X && !pressXButton_) {
				if (havingOrbs_[selectedChangeOrb_] != selectedChangeType_) {
					// 所持しているオーブリストの指定された位置に新しいオーブを挿入
					havingOrbs_.insert(
					    havingOrbs_.begin() + selectedChangeOrb_,
					    (PlayerBullet::BulletType)selectedChangeType_);
					// 変換に使用したオーブを削除
					havingOrbs_.erase(havingOrbs_.begin() + selectedChangeOrb_ + 1);
					// 変換に必要な敵数をリセット
					needChangeOrbEnemyCount_ = kNeedChangeOrbEnemyCount_;
				}
			}
		}
	}
}

void Player::PlayCameraShake(Vector2 shakeStrength, float shakeTime) {
	// 引数の値をメンバ変数に代入
	shakeStrength_ = shakeStrength;
	shakeTime_ = shakeTime;

	// 演出用tをリセット
	shakeT_ = 0.0f;
	// カメラ振動有効
	enableCameraShake_ = true;
}

void Player::CameraShake() {
	// 指定秒数カメラシェイク
	if (shakeT_ <= shakeTime_) {
		// カメラシェイクをイージングで表現
		handOverCameraShakeStrength_.x = MyMath::EaseOut(
		    shakeT_, MyMath::RandomF(-shakeStrength_.x, shakeStrength_.x, 2), 0.0f, shakeTime_);
		handOverCameraShakeStrength_.y = MyMath::EaseOut(
		    shakeT_, MyMath::RandomF(-shakeStrength_.y, shakeStrength_.y, 2), 0.0f, shakeTime_);
		// 演出t加算
		shakeT_ += 1.0f / 60.0f;
	} else {
		// カメラシェイク強さリセット
		handOverCameraShakeStrength_ = {0.0f, 0.0f};
		enableCameraShake_ = false;
	}
}

void Player::TranslationLimit() {

	// トランスフォームの制限確認
	Field* field = Field::GetInstance();
	worldTransform_.translation_.x =
	    std::clamp(worldTransform_.translation_.x, field->GetMin().x, field->GetMax().x);

	worldTransform_.translation_.y =
	    std::clamp(worldTransform_.translation_.y, field->GetMin().y, field->GetMax().y);

	worldTransform_.translation_.z =
	    std::clamp(worldTransform_.translation_.z, field->GetMin().z, field->GetMax().z);

}

void Player::UIUpdate() {

	// UIの設定
	for (int i = 0; i < 3; i++) {
		// スプライトの生成
		spriteHavingOrbs_[i]->SetPosition(
		    {spriteHavingOrbsStartPos_.x + (spriteHavingOrbsLineSpace_.x * i),
		     spriteHavingOrbsStartPos_.y + (spriteHavingOrbsLineSpace_.y * i)});
		// サイズ設定
		spriteHavingOrbs_[i]->SetSize(spriteHavingOrbsSize_);

		// とりあえずの色設定
		spriteHavingOrbs_[i]->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
	}

	// 所持しているオーブ数を確認
	int havingOrbCount = (int)havingOrbs_.size();
	for (int i = 0; i < havingOrbCount; i++) {
		// 該当するオーブの色を取得
		PlayerBullet::BulletType haveOrb = havingOrbs_[i];
		switch (haveOrb) {
		case PlayerBullet::Fire: // 赤の場合
			// スプライトの色を設定
			spriteHavingOrbs_[i]->SetColor({1.0f, 0.0f, 0.05f, 1.0f});
			break;
		case PlayerBullet::Ice: // 青の場合
			// スプライトの色を設定
			spriteHavingOrbs_[i]->SetColor({0.15f, 0.7f, 1.0f, 1.0f});
			break;
		case PlayerBullet::Thunder: // 黄の場合
			spriteHavingOrbs_[i]->SetColor({1.0f, 0.8f, 0.025f, 1.0f});
			break;
		}
	}

	// 選択UIの座標調整
	spriteSelectedOrbs_->SetPosition(
	    {spriteHavingOrbsStartPos_.x + (spriteHavingOrbsLineSpace_.x * selectedChangeOrb_),
	     spriteHavingOrbsStartPos_.y + (spriteHavingOrbsLineSpace_.y * selectedChangeOrb_)});

	// 変換テキストUIの座標調整
	spriteChangeOrbText_->SetPosition(spriteChangeOrbUI_.position_);

	// 変換するオーブの種類によってスプライトを変化
	switch (selectedChangeType_) {
	case PlayerBullet::Fire:
		// 炎弾UIを選択状態に
		spriteDpadUP_->SetTextureHandle(textureHandles_[TextureManager::FireBullet]);
		spriteDpadUP_->SetColor({1.0f, 0.0f, 0.05f, 1.0f});
		break;
	case PlayerBullet::Ice:
		// 氷弾UIを選択状態に
		spriteDpadUP_->SetTextureHandle(textureHandles_[TextureManager::IceBullet]);
		spriteDpadUP_->SetColor({0.15f, 0.7f, 1.0f, 1.0f});
		break;
	case PlayerBullet::Thunder:
		// 雷弾UIを選択状態に
		spriteDpadUP_->SetTextureHandle(textureHandles_[TextureManager::ThunderBullet]);
		spriteDpadUP_->SetColor({1.0f, 0.8f, 0.025f, 1.0f});
		break;
	default:
		break;
	}

	// オーブが２つ以上ある場合矢印を
	if (havingOrbCount >= 2) {
		if (needChangeOrbEnemyCount_ <= 0) {
			// 矢印UIを明るい状態に
			spriteChangeOrbText2_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
		}
		spriteDpadLeft_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
		spriteDpadRight_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
	} 
	else {
		// 矢印UIを暗い状態に
		spriteChangeOrbText2_->SetColor({0.45f, 0.45f, 0.45f, 1.0f});
		spriteDpadLeft_->SetColor({0.45f, 0.45f, 0.45f, 1.0f});
		spriteDpadRight_->SetColor({0.45f, 0.45f, 0.45f, 1.0f});
	}

	// 次の特殊射撃を元にスプライトを変更
	switch (specialShotBulletPlans_) {
	case PlayerBullet::None:
		// 撃てない
		spriteSpecialShotPlan_->SetTextureHandle(textureHandles_[TextureManager::None]);
		spriteSpecialShotPlan_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
		break;
	case PlayerBullet::Fire:
		// 炎弾
		spriteSpecialShotPlan_->SetTextureHandle(textureHandles_[TextureManager::FireBullet]);
		spriteSpecialShotPlan_->SetColor({1.0f, 0.0f, 0.05f, 1.0f});
		break;
	case PlayerBullet::Ice:
		// 氷弾
		spriteSpecialShotPlan_->SetTextureHandle(textureHandles_[TextureManager::IceBullet]);
		spriteSpecialShotPlan_->SetColor({0.15f, 0.7f, 1.0f, 1.0f});
		break;
	case PlayerBullet::Thunder:
		// 雷弾
		spriteSpecialShotPlan_->SetTextureHandle(textureHandles_[TextureManager::ThunderBullet]);
		spriteSpecialShotPlan_->SetColor({1.0f, 0.8f, 0.025f, 1.0f});
		break;
	}

	// 特殊射撃の強さを元にスプライトを変更
	switch (specialShotStrength_) {
	case 1:
		spriteSpecialShotMagnification_->SetTextureHandle(
		    textureHandles_[TextureManager::Texturex1]);
		break;
	case 2:
		spriteSpecialShotMagnification_->SetTextureHandle(
		    textureHandles_[TextureManager::Texturex2]);
		break;
	case 3:
		spriteSpecialShotMagnification_->SetTextureHandle(
		    textureHandles_[TextureManager::Texturex3]);
		break;
	default:
		break;
	}

	// 変換に必要な敵数を元にスプライトを変更
	switch (needChangeOrbEnemyCount_) { 
		case 0:
		// スプライトを変更
		spriteNeedChangeOrbEnemyCount_->SetTextureHandle(
		    textureHandles_[TextureManager::texture_0]);
		break;
		case 1:
		// スプライトを変更
		spriteNeedChangeOrbEnemyCount_->SetTextureHandle(
		    textureHandles_[TextureManager::texture_1]);
		break;
		case 2:
		// スプライトを変更
		spriteNeedChangeOrbEnemyCount_->SetTextureHandle(
		    textureHandles_[TextureManager::texture_2]);
		break;
		case 3:
		// スプライトを変更
		spriteNeedChangeOrbEnemyCount_->SetTextureHandle(
		    textureHandles_[TextureManager::texture_3]);
		break;
		case 4:
		// スプライトを変更
		spriteNeedChangeOrbEnemyCount_->SetTextureHandle(
		    textureHandles_[TextureManager::texture_4]);
		break;
		case 5:
		// スプライトを変更
		spriteNeedChangeOrbEnemyCount_->SetTextureHandle(
		    textureHandles_[TextureManager::texture_5]);
		break;
		case 6:
		// スプライトを変更
		spriteNeedChangeOrbEnemyCount_->SetTextureHandle(
		    textureHandles_[TextureManager::texture_6]);
		break;
		case 7:
		// スプライトを変更
		spriteNeedChangeOrbEnemyCount_->SetTextureHandle(
		    textureHandles_[TextureManager::texture_7]);
		break;
		case 8:
		// スプライトを変更
		spriteNeedChangeOrbEnemyCount_->SetTextureHandle(
		    textureHandles_[TextureManager::texture_8]);
		break;
		case 9:
		// スプライトを変更
		spriteNeedChangeOrbEnemyCount_->SetTextureHandle(
		    textureHandles_[TextureManager::texture_9]);
		break;
	default:
		break;
	}

	// ゲームパッドの状態取得
	XINPUT_STATE joyState;
	if (input_->GetJoystickState(0, joyState)) {

		// Rトリガーが押されたら
		if (joyState.Gamepad.bRightTrigger > triggerDeadZone_R_) {
			spriteRightTrigger_->SetTextureHandle(textureHandles_[TextureManager::RT_P]);
		} else {
			spriteRightTrigger_->SetTextureHandle(textureHandles_[TextureManager::RT_N]);
		}

		// テキストUIのテクスチャを変更
		spriteChangeOrbText_->SetTextureHandle(textureHandles_[TextureManager::changeOrbText]);
		// テクスチャ描画範囲設定
		spriteChangeOrbText_->SetTextureRect({0.0f, 0.0f}, {1280.0f, 256.0f});
		spriteChangeOrbText_->SetPosition({230.0f, 475.0f}); // 座標
		spriteChangeOrbText_->SetSize({352.0f, 88.0f});      // 大きさ

		// 左トリガー
		if (joyState.Gamepad.bLeftTrigger > triggerDeadZone_L_) {
			// ボタンを押すとテクスチャを変更
			spriteLeftTrigger_->SetTextureHandle(textureHandles_[TextureManager::LT_P]);
		} else {
			// ボタンを押すとテクスチャを変更
			spriteLeftTrigger_->SetTextureHandle(textureHandles_[TextureManager::LT_N]);
		}

		if (!canSpecialShot_) {
			spriteLeftTrigger_->SetColor({0.65f, 0.65f, 0.65f, 1.0f});
			specialShotText_->SetColor({0.65f, 0.65f, 0.65f, 1.0f});
			spriteSpecialShotMagnification_->SetColor({0.65f, 0.65f, 0.65f, 1.0f});
		} else {
			spriteLeftTrigger_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
			specialShotText_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
			spriteSpecialShotMagnification_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
		}

	}

}

void Player::ApplyGlobalVariables() {

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "Player";

	// メンバ変数の調整項目をグローバル変数に追加
	height_ = globalVariables->GetFloatValue(groupName, "Height"); // 身長
	moveSpeed_ = globalVariables->GetFloatValue(groupName, "MoveSpeed"); // 移動速度
	kMaxFallSpeed_ = globalVariables->GetFloatValue(groupName, "MaxFallSpeed"); // 最大落下速度
	kFallAcceleration_ = globalVariables->GetFloatValue(groupName, "FallAcceleration"); // 落下加速度
	kMaxJumpHeight_ = globalVariables->GetFloatValue(groupName, "MaxJumpHeight"); // 最大ジャンプ高度
	kJumpDecayRate_ = globalVariables->GetFloatValue(groupName, "JumpDecayRate"); // ジャンプ減衰速度
	shotPosOffset_ = globalVariables->GetVector3Value(groupName, "ShotPosOffset"); // 射撃座標オフセット
	kDistanceToReticleObject_ = globalVariables->GetFloatValue(groupName, "DistanceToReticleObject"); // カメラから照準オブジェクトの距離
	bulletSpeed_ = globalVariables->GetFloatValue(groupName, "BulletSpeed"); // 弾速
	kMaxFireCoolTime_ = globalVariables->GetFloatValue(groupName, "MaxFireCoolTime"); // 発射レート
	kMaxMagazine_ = globalVariables->GetIntValue(groupName, "MaxMagazineSize"); // 最大弾数設定
	kMaxReloadTime_ = globalVariables->GetIntValue(groupName, "MaxReloadTime"); // リロードにかかる時間
	spriteHavingOrbsStartPos_ =
	    globalVariables->GetVector2Value(groupName, "SpriteHavingOrbsStartPos"); // 所持オーブ描画UIの開始座標
	spriteHavingOrbsSize_ = 
		globalVariables->GetVector2Value(groupName, "SpriteHavingOrbsSize"); // 所持オーブ描画UIの大きさ
	spriteHavingOrbsLineSpace_ = 
		globalVariables->GetVector2Value(groupName, "SpriteHavingOrbsLineSpace"); // 所持オーブ描画UIの行間

	//バレットダメージ

	bulletDamage_[PlayerBullet::BulletType::None] =
	    uint32_t(globalVariables->GetIntValue(groupName, "BulletDamageNone"));
	bulletDamage_[PlayerBullet::BulletType::Fire] =
	    uint32_t(globalVariables->GetIntValue(groupName, "BulletDamageFire"));
	bulletDamage_[PlayerBullet::BulletType::Ice] =
	    uint32_t(globalVariables->GetIntValue(groupName, "BulletDamageIce"));
	bulletDamage_[PlayerBullet::BulletType::Thunder] =
	    uint32_t(globalVariables->GetIntValue(groupName, "BulletDamageThunder"));

	collisionInvincibilityTime_ =
	    uint32_t(globalVariables->GetIntValue(groupName, "CollisionInvincibilityTime"));

	colliderShape_->ApplyGlobalVariables(groupName);
	

#ifdef _DEBUG
	
#endif // _DEBUG
}
