#include "Camera.h"
#include "config/GlobalVariables.h"

void Camera::Intialize() {
	// ビュープロジェクション初期化
	viewProjection_.farZ = 2500.0f;
	viewProjection_.Initialize();
	viewProjection_.translation_ = {0.0f, 5.0f, 0.0f};

	// 入力取得
	input_ = Input::GetInstance();

	// カメラFOV
	fov_ = 0.45f;
	// カメラ感度
	cameraSensitivity_ = {0.025f, 0.025f};

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "Camera";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加
	globalVariables->AddItem(groupName, "FOV", fov_);
	globalVariables->AddItem(groupName, "Sensitivity", cameraSensitivity_);

}

void Camera::Update(bool isOption) {

	// 視野角の設定
	viewProjection_.fovAngleY = fov_;

	// ゲームパッドの状態取得
	XINPUT_STATE joyState;
	if (input_->GetJoystickState(0, joyState) && !isOption) {
		// スティックの横移動に対してカメラを旋回
		viewProjection_.rotation_.x -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * cameraSensitivity_.x;
		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * cameraSensitivity_.y;

		// 角度制限
		if (viewProjection_.rotation_.x <= -1.50f) {
			viewProjection_.rotation_.x = -1.50f;
		} else if (viewProjection_.rotation_.x >= 1.50f) {
			viewProjection_.rotation_.x = 1.50f;
		}

		// カメラ回転角のリセット
		if (viewProjection_.rotation_.x >= (float)std::numbers::pi * 2.0f ||
		    (float)-std::numbers::pi * 2.0f >= viewProjection_.rotation_.x)
			viewProjection_.rotation_.x = 0.0f;
		if (viewProjection_.rotation_.y >= (float)std::numbers::pi * 2.0f ||
		    (float)-std::numbers::pi * 2.0f >= viewProjection_.rotation_.y)
			viewProjection_.rotation_.y = 0.0f;
		if (viewProjection_.rotation_.z >= (float)std::numbers::pi * 2.0f ||
		    (float)-std::numbers::pi * 2.0f >= viewProjection_.rotation_.z)
			viewProjection_.rotation_.z = 0.0f;
	}

	// 追従対象が存在すれば
	if (target_) {

		// 追従対象からカメラまでのオフセットを設定
		Vector3 offset = {0.0f, 0.0f, -3.0f};
		// カメラシェイク強さを取得
		Vector2 shakeStrength = (Vector2)*shakeStrength_;
		offset.x += shakeStrength.x;
		offset.y += shakeStrength.y;

		// カメラの角度から回転行列を生成
		Matrix4x4 rotateMat = MyMath::MakeRotateXYZMatrix(viewProjection_.rotation_);

		// オフセットをカメラの回転に合わせて回転させる
		offset = MyMath::Transform(offset, rotateMat);

		// 座標コピーからオフセット分ずらす
		viewProjection_.translation_ = target_->translation_ + offset;
	}

	// 調整項目の適用
	//ApplyGlobalVariables();
	fov_ = std::clamp(fov_, fovMin_, fovMax_);
	cameraSensitivity_.x =
	    std::clamp(cameraSensitivity_.x, cameraSensitivityMin_, cameraSensitivityMax_);
	cameraSensitivity_.y =
	    std::clamp(cameraSensitivity_.y, cameraSensitivityMin_, cameraSensitivityMax_);

	// ビュープロジェクション行列の更新
	viewProjection_.UpdateMatrix();
}

void Camera::SetUp() {

	viewProjection_.Initialize();
	viewProjection_.translation_ = {0.0f, 5.0f, 0.0f};
	viewProjection_.rotation_ = {0.0f, 0.0f, 0.0f};

}

void Camera::ApplyGlobalVariables() {
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "Camera";

	// メンバ変数の調整項目をグローバル変数に追加
	fov_ = globalVariables->GetFloatValue(groupName, "FOV");                 // 視野角
	cameraSensitivity_ = globalVariables->GetVector2Value(groupName, "Sensitivity"); // 移動速度
}
