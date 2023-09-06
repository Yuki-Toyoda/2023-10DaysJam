#include "Player.h"
#include "../../config/GlobalVariables.h"
#include "Collision/ColliderShape/Sphere.h"

#ifdef _DEBUG

#endif // _DEBUG

void Player::Initialize(
    const std::vector<Model*>& modelsPlayer, const std::vector<Model*>& modelsBullet) {

	// 基底クラス初期化
	BaseCharacter::Initialize(modelsPlayer);

	// 弾モデル読み込み
	modelBullet_ = modelsBullet;

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

	// 3Dレティクルワールド座標の初期化
	worldTransform3DReticle_.Initialize();

	// 身長高さ
	height_ = 5.0f;

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
	kMaxFireCoolTime_ = 25;
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

#pragma region ImGuiテスト用変数
#ifdef _DEBUG

	// 追加するオーブの種類リセット
	selectOrbs_ = PlayerBullet::Fire;

#endif // _DEBUG
#pragma endregion

	// 衝突属性を設定
	SetCollisionAttribute(0xfffffffe);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(0x00000001);

	// コライダーの形
	Sphere* sphere = new Sphere();
	sphere->Initialize(GetWorldPosition(), 1.0f);
	colliderShape_ = sphere;
	
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
	globalVariables->AddItem(groupName, "DistanceToReticleObject", kDistanceToReticleObject_); // カメラから照準オブジェクトの距離
	globalVariables->AddItem(groupName, "BulletSpeed", bulletSpeed_); // 弾速
	globalVariables->AddItem(groupName, "MaxMagazineSize", kMaxMagazine_); // 最大弾数設定
	globalVariables->AddItem(groupName, "MaxReloadTime", kMaxReloadTime_); // リロードにかかる時間

	colliderShape_->AddToGlobalVariables(groupName);

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
	}

	// 調整項目を反映
	ApplyGlobalVariables();

	// 基底クラス更新
	BaseCharacter::Update();

	// コライダー更新
	colliderShape_->Update(GetWorldPosition(), 1.0f);

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
	}

	#ifdef _DEBUG

	ImGui::Begin("player");

	// 追加するオーブを選択
	ImGui::RadioButton("Fire", &selectOrbs_, PlayerBullet::Fire);
	ImGui::SameLine();
	ImGui::RadioButton("Water", &selectOrbs_, PlayerBullet::Water);
	ImGui::SameLine();
	ImGui::RadioButton("Thunder", &selectOrbs_, PlayerBullet::Thunder);

	if (ImGui::Button("AddOrbs")) {
		// 選択したオーブを追加
		AddOrbs((PlayerBullet::BulletType)selectOrbs_);
	}

	for (int i = 0; i < havingOrbs_.size(); i++) {
		int OrbType = havingOrbs_[i];
		ImGui::DragInt("havingOrbType", &OrbType, 1.0f);
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

void Player::OnCollision() {

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

		// 移動
		worldTransform_.translation_ = worldTransform_.translation_ + move;
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
				Vector3 shotPos = BaseCharacter::GetWorldPosition();
				shotPos = shotPos + shotPosOffset_;
				shotVelocity = ReticleWorldPos - shotPos;
				shotVelocity = MyMath::Normalize(shotVelocity) * bulletSpeed_;

				// 生成した弾を初期化
				newBullet->Initialize(
				    modelBullet_,  // 3Dモデル
				    shotPos,                    // 初期位置
					viewProjection_->rotation_, // 初期角度
				    shotVelocity, PlayerBullet::Normal); // 弾速

				// 生成した弾をリストに入れる
				bullets_.push_back(newBullet);

				// 弾数デクリメント
				magazine_--;

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
	// ゲームパッドの状態取得
	XINPUT_STATE joyState;
	if (input_->GetJoystickState(0, joyState)) {
		// 左トリガーが押されたら特殊射撃
		if (joyState.Gamepad.bLeftTrigger > triggerDeadZone_L_ && !pressLTrigger_) {
			if (canSpecialShot_) {
			
			}
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

#ifdef _DEBUG
	
#endif // _DEBUG
}
