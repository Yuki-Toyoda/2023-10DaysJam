#include "Player.h"

#ifdef _DEBUG
#include "../../config/GlobalVariables.h"
#endif // _DEBUG

void Player::Initialize(const std::vector<Model*>& models) {
	// 基底クラス初期化
	BaseCharacter::Initialize(models);

	// 入力情報取得
	input_ = Input::GetInstance();

	// 初期座標を設定
	worldTransform_.translation_ = {0.0f, 10.0f, 0.0f};

	// 移動速度初期化
	moveSpeed_ = 3.0f;

	#ifdef _DEBUG

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "Player";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加
	globalVariables->AddItem(groupName, "Test", 0.0f);
	globalVariables->AddItem(groupName, "moveSpeed", moveSpeed_);

#endif // _DEBUG
}

void Player::Update() {

	// 移動処理
	Move();

	// 調整項目を反映
	ApplyGlobalVariables();

	// 基底クラス更新
	BaseCharacter::Update();
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

}

void Player::ApplyGlobalVariables() {
#ifdef _DEBUG
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "Player";

	// メンバ変数の調整項目をグローバル変数に追加
	moveSpeed_ = globalVariables->GetFloatValue(groupName, "moveSpeed");
#endif // _DEBUG
}
