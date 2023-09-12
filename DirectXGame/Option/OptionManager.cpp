#include "OptionManager.h"
#include "Camera/Camera.h"
#include <imgui.h>

OptionManager* OptionManager::GetInstance() {
	static OptionManager instance;
	return &instance;
}

void OptionManager::Initialize(Camera* camera) {

	//カメラ設定
	camera_ = camera;

	//開かれているか
	isOpen_ = false;

	// 現在の項目
	itemNow = Fov;

	// クールタイム
	besideButtonCooltimer_ = 0;

}

void OptionManager::Update(_XINPUT_GAMEPAD gamePad, _XINPUT_GAMEPAD preGamePad) {


	Vector2 cameraSensitivityPulsValue = {0.0f, 0.0f};

	//タイマー
	if (besideButtonCooltimer_ > 0) {
		besideButtonCooltimer_--;
	}

	//十字上
	if (gamePad.wButtons & XINPUT_GAMEPAD_DPAD_UP &&
	    !(preGamePad.wButtons & XINPUT_GAMEPAD_DPAD_UP)) {

		// 項目変更
		switch (itemNow) {
		case OptionManager::Fov:
			itemNow = CameraSensitivityY;
			break;
		case OptionManager::CameraSensitivityX:
			itemNow = Fov;
			break;
		case OptionManager::CameraSensitivityY:
			itemNow = CameraSensitivityX;
			break;
		default:
			break;
		}

	} 
	// 十字下
	else if (gamePad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN &&
	    !(preGamePad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)) {
	
		// 項目変更
		switch (itemNow) {
		case OptionManager::Fov:
			itemNow = CameraSensitivityX;
			break;
		case OptionManager::CameraSensitivityX:
			itemNow = CameraSensitivityY;
			break;
		case OptionManager::CameraSensitivityY:
			itemNow = Fov;
			break;
		default:
			break;
		}

	}
	// 十字右
	else if (
	    gamePad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT &&
		besideButtonCooltimer_ == 0) {

		besideButtonCooltimer_ = kBesideButtonCooltime_;

		// 項目変更
		switch (itemNow) {
		case OptionManager::Fov:
			camera_->SetFov(camera_->GetFov() + fovPulsValue_);
			break;
		case OptionManager::CameraSensitivityX:
			cameraSensitivityPulsValue = camera_->GetCameraSensitivity();
			cameraSensitivityPulsValue.x += cameraSensitivityPulsValue_;
			camera_->SetCameraSensitivity(cameraSensitivityPulsValue);
			break;
		case OptionManager::CameraSensitivityY:
			cameraSensitivityPulsValue = camera_->GetCameraSensitivity();
			cameraSensitivityPulsValue.y += cameraSensitivityPulsValue_;
			camera_->SetCameraSensitivity(cameraSensitivityPulsValue);
			break;
		default:
			break;
		}
	}
	// 十字左
	else if (
	    gamePad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT &&
		besideButtonCooltimer_ == 0) {

		besideButtonCooltimer_ = kBesideButtonCooltime_;

		// 項目変更
		switch (itemNow) {
		case OptionManager::Fov:
			camera_->SetFov(camera_->GetFov() - fovPulsValue_);
			break;
		case OptionManager::CameraSensitivityX:
			cameraSensitivityPulsValue = camera_->GetCameraSensitivity();
			cameraSensitivityPulsValue.x -= cameraSensitivityPulsValue_;
			camera_->SetCameraSensitivity(cameraSensitivityPulsValue);
			break;
		case OptionManager::CameraSensitivityY:
			cameraSensitivityPulsValue = camera_->GetCameraSensitivity();
			cameraSensitivityPulsValue.y -= cameraSensitivityPulsValue_;
			camera_->SetCameraSensitivity(cameraSensitivityPulsValue);
			break;
		default:
			break;
		}
	}


	#ifdef _DEBUG

	ImGui::Begin("Option");
	ImGui::Text("Fov%f", camera_->GetFov());
	ImGui::Text("CameraSensitivityX%f", camera_->GetCameraSensitivity().x);
	ImGui::Text("CameraSensitivityY%f", camera_->GetCameraSensitivity().y);
	ImGui::End();

#endif // DEBUG

}

void OptionManager::Draw() {


}

void OptionManager::OpenClose(_XINPUT_GAMEPAD gamePad, _XINPUT_GAMEPAD preGamePad) {

	//スタートボタンがおされたら切り替える
	if (gamePad.wButtons & XINPUT_GAMEPAD_START && 
			!(preGamePad.wButtons & XINPUT_GAMEPAD_START)) {
			
		//開くまたは閉じる
		if (isOpen_) {
			isOpen_ = false;
		} else {
			isOpen_ = true;
		}

	}

}
