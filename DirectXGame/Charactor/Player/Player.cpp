#include "Player.h"
#include "../../config/GlobalVariables.h"

#ifdef _DEBUG

#endif // _DEBUG

void Player::Initialize(const std::vector<Model*>& models) {
	// 基底クラス初期化
	BaseCharacter::Initialize(models);

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

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "Player";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加
	globalVariables->AddItem(groupName, "Height", height_); // 身長
	globalVariables->AddItem(groupName, "MoveSpeed", moveSpeed_); // 移動速度
	globalVariables->AddItem(groupName, "MaxFallSpeed", kMaxFallSpeed_);  // 最大落下速度
	globalVariables->AddItem(groupName, "FallAcceleration", kFallAcceleration_); // 落下加速度
	globalVariables->AddItem(groupName, "MaxJumpHeight", kMaxJumpHeight_);  // 最大ジャンプ高度
	globalVariables->AddItem(groupName, "JumpDecayRate", kJumpDecayRate_); // ジャンプ減衰速度

}

void Player::Update() {

	// 行動可能なら
	if (canAction_) {
		// 移動処理
		Move();
		// ジャンプ処理
		Jump();
	}

	// 調整項目を反映
	ApplyGlobalVariables();

	// 基底クラス更新
	BaseCharacter::Update();

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
	}
}

void Player::Draw(const ViewProjection& viewProjection) { viewProjection; }

void Player::OnCollision() {
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

#ifdef _DEBUG
	
#endif // _DEBUG
}
