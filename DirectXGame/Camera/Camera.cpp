#include "Camera.h"

void Camera::Intialize() {
	// ビュープロジェクション初期化
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
	viewProjection_.translation_ = {0.0f, 5.0f, 0.0f};

	// 入力取得
	input_ = Input::GetInstance();

	// カメラ感度
	cameraSensitivity_ = {0.025f, 0.025f};
}

void Camera::Update() {
	// ゲームパッドの状態取得
	XINPUT_STATE joyState;
	if (input_->GetJoystickState(0, joyState)) {
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

		// カメラの角度から回転行列を生成
		Matrix4x4 rotateMat = MyMath::MakeRotateXYZMatrix(viewProjection_.rotation_);

		// オフセットをカメラの回転に合わせて回転させる
		offset = MyMath::Transform(offset, rotateMat);

		// 座標コピーからオフセット分ずらす
		viewProjection_.translation_ = target_->translation_ + offset;
	}

	// ビュープロジェクション行列の更新
	viewProjection_.UpdateMatrix();
}
