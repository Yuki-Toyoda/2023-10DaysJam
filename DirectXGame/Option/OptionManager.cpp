#include "OptionManager.h"
#include "Camera/Camera.h"
#include <imgui.h>

OptionManager* OptionManager::GetInstance() {
	static OptionManager instance;
	return &instance;
}

void OptionManager::Initialize(
    Camera* camera, const std::vector<uint32_t>& textureHandles,
    const std::vector<uint32_t>& audioHandles) {

	// 音をうけとろう
	audio_ = Audio::GetInstance();

	// テクスチャ受け取り
	textureHandles_ = textureHandles;
	// 効果音受け取り
	audioHandles_ = audioHandles;

	// カメラ設定
	camera_ = camera;

	// 開かれているか
	isOpen_ = false;

	// 現在の項目
	itemNow = Fov;

	// クールタイム
	besideButtonCooltimer_ = 0;

	// 背景スプライトリセット
	spriteBackGround_.reset(Sprite::Create(
	    textureHandles_[TextureManager::BackGruond], {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f},
	    {0.0f, 0.0f}));

	// 視野角スプライト座標
	spriteFOVPosition_ = {300.0f, 225.0f};
	// 視野角項目スプライト
	spriteFOV_.reset(Sprite::Create(
	    textureHandles_[TextureManager::FOV], spriteFOVPosition_,
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	// ゲージのX座標設定
	float GagePosX = (spriteFOVPosition_.x + (512.0f / 2.0f)) + 20.0f;
	// 視野角ゲージ背景
	spriteFOVGageBackGround_.reset(Sprite::Create(
	    textureHandles_[TextureManager::texture1x1], {GagePosX, spriteFOVPosition_.y},
	    {0.0f, 0.0f, 0.0f, 1.0f},
	    {0.0f, 0.5f}));
	spriteFOVGageBackGround_->SetSize(GageLength);
	// 視野角ゲージ
	spriteFOVGage_.reset(Sprite::Create(
	    textureHandles_[TextureManager::texture1x1], {GagePosX, spriteFOVPosition_.y},
	    {0.0f, 0.8f, 0.2f, 1.0f}, {0.0f, 0.5f}));
	spriteFOVGage_->SetSize({0.0f, GageLength.y});

	// 水平感度スプライト座標
	spriteCameraSensitivityXPosition_ = {300.0f, 340.0f};
	// 水平感度スプライト
	spriteCameraSensitivityX_.reset(Sprite::Create(
	    textureHandles_[TextureManager::CameraSensitivityX], spriteCameraSensitivityXPosition_,
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	// 水平感度ゲージ背景
	spriteCameraSensitivityXGageBackGround_.reset(Sprite::Create(
	    textureHandles_[TextureManager::texture1x1],
	    {GagePosX, spriteCameraSensitivityXPosition_.y}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.5f}));
	spriteCameraSensitivityXGageBackGround_->SetSize(GageLength);
	// 水平感度ゲージ
	spriteCameraSensitivityXGage_.reset(Sprite::Create(
	    textureHandles_[TextureManager::texture1x1],
	    {GagePosX, spriteCameraSensitivityXPosition_.y},
	    {0.0f, 0.8f, 0.2f, 1.0f}, {0.0f, 0.5f}));
	spriteCameraSensitivityXGage_->SetSize({0.0f, GageLength.y});

	// 垂直感度スプライト座標
	spriteCameraSensitivityYPosition_ = {300.0f, 455.0f};
	// 垂直感度スプライト
	spriteCameraSensitivityY_.reset(Sprite::Create(
	    textureHandles_[TextureManager::CameraSensitivityY], spriteCameraSensitivityYPosition_,
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
	// 垂直感度ゲージ背景
	spriteCameraSensitivityYGageBackGround_.reset(Sprite::Create(
	    textureHandles_[TextureManager::texture1x1],
	    {GagePosX, spriteCameraSensitivityYPosition_.y}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.5f}));
	spriteCameraSensitivityYGageBackGround_->SetSize(GageLength);
	// 垂直感度ゲージ
	spriteCameraSensitivityYGage_.reset(Sprite::Create(
	    textureHandles_[TextureManager::texture1x1],
	    {GagePosX, spriteCameraSensitivityYPosition_.y}, {0.0f, 0.8f, 0.2f, 1.0f}, {0.0f, 0.5f}));
	spriteCameraSensitivityYGage_->SetSize({0.0f, GageLength.y});

	// 選択項目スプライト座標
	spriteSelectItemPosition_ = spriteFOVPosition_;
	// 選択項目スプライト
	spriteSelectItem_.reset(Sprite::Create(
	    textureHandles_[TextureManager::SelectItem], spriteSelectItemPosition_,
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	// 番号開始座標
	spriteNumberStartPos_ = {spriteFOVPosition_.x + 800.0f, spriteFOVPosition_.y};
	// 数値テクスチャリセット
	for (int i = 0; i < 3; i++) {
		spriteNumbers_[i].reset(Sprite::Create(textureHandles_[0], 
			{spriteNumberStartPos_.x + (32.0f * i), spriteNumberStartPos_.y}, 
			{1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));
		spriteNumbers_[i]->SetSize({64.0f, 64.0f});
	}

}

void OptionManager::Update(_XINPUT_GAMEPAD gamePad, _XINPUT_GAMEPAD preGamePad) {

	Vector2 cameraSensitivityPulsValue = {0.0f, 0.0f};

	//タイマー
	if (besideButtonCooltimer_ > 0) {
		besideButtonCooltimer_--;
	}

	//十字上
	if (gamePad.wButtons & XINPUT_GAMEPAD_DPAD_UP &&
	        !(preGamePad.wButtons & XINPUT_GAMEPAD_DPAD_UP) ||
	    Input::GetInstance()->TriggerKey(DIK_W)) {

		// 次へ音再生
		audio_->PlayWave(audioHandles_[Audio::MoveItem]);

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
	        !(preGamePad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ||
	    Input::GetInstance()->TriggerKey(DIK_S)) {
	
		// 次へ音再生
		audio_->PlayWave(audioHandles_[Audio::MoveItem]);

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
	    gamePad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT && besideButtonCooltimer_ == 0 ||
	    Input::GetInstance()->PushKey(DIK_D)) {

		// 値変更音再生
		audio_->PlayWave(audioHandles_[Audio::ChangeValue]);

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
	    gamePad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT && besideButtonCooltimer_ == 0 ||
	    Input::GetInstance()->PushKey(DIK_A)) {

		// 値変更音再生
		audio_->PlayWave(audioHandles_[Audio::ChangeValue]);

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

	// 視野角ゲージを動かす
	float GageLengthX = MyMath::Linear(camera_->GetFov() - 0.45f, 10.0f, GageLength.x, 1.2f - 0.45f);
	spriteFOVGage_->SetSize({GageLengthX, GageLength.y});
	// 水平感度ゲージを動かす
	GageLengthX = MyMath::Linear(
	    camera_->GetCameraSensitivity().x - 0.001f, 10.0f, GageLength.x, 0.1f - 0.001f);
	spriteCameraSensitivityXGage_->SetSize({GageLengthX, GageLength.y});
	// 垂直感度ゲージを動かす
	GageLengthX = MyMath::Linear(
	    camera_->GetCameraSensitivity().y - 0.001f, 10.0f, GageLength.x, 0.1f - 0.001f);
	spriteCameraSensitivityYGage_->SetSize({GageLengthX, GageLength.y});

	float FOV = camera_->GetFov();
	int finalFOV = (int)(FOV * 100.0f);

	Vector2 cameraSensitivity = camera_->GetCameraSensitivity();
	int finalCameraSesitivityX = (int)(cameraSensitivity.x * 1000.0f);
	int finalCameraSesitivityY = (int)(cameraSensitivity.y * 1000.0f);

	// 選択されている項目によってスプライトを移動
	switch (itemNow) {
	case OptionManager::Fov:
		// 選択スプライトを選択項目に合わせる
		spriteSelectItemPosition_ = spriteFOVPosition_;
		for (int i = 0; i < 3; i++) {
			spriteNumbers_[i]->SetPosition(
			    {spriteNumberStartPos_.x + (32.0f * i), spriteFOVPosition_.y});
			/* n桁目以下の桁を取得 */
			int result = finalFOV % (int)pow(10, 3 - i);

			/* m桁目以上の桁を取得 */
			result = result / (int)pow(10, 3 - i - 1);

			spriteNumbers_[i]->SetTextureHandle(textureHandles_[result]);
		}

		break;
	case OptionManager::CameraSensitivityX:
		// 選択スプライトを選択項目に合わせる
		spriteSelectItemPosition_ = spriteCameraSensitivityXPosition_;
		for (int i = 0; i < 3; i++) {
			spriteNumbers_[i]->SetPosition(
			    {spriteNumberStartPos_.x + (32.0f * i), spriteCameraSensitivityXPosition_.y});
			/* n桁目以下の桁を取得 */
			int result = finalCameraSesitivityX % (int)pow(10, 3 - i);

			/* m桁目以上の桁を取得 */
			result = result / (int)pow(10, 3 - i - 1);

			spriteNumbers_[i]->SetTextureHandle(textureHandles_[result]);
		}
		break;
	case OptionManager::CameraSensitivityY:
		// 選択スプライトを選択項目に合わせる
		spriteSelectItemPosition_ = spriteCameraSensitivityYPosition_;	
		for (int i = 0; i < 3; i++) {
			spriteNumbers_[i]->SetPosition(
			    {spriteNumberStartPos_.x + (32.0f * i), spriteCameraSensitivityYPosition_.y});
			/* n桁目以下の桁を取得 */
			int result = finalCameraSesitivityY % (int)pow(10, 3 - i);

			/* m桁目以上の桁を取得 */
			result = result / (int)pow(10, 3 - i - 1);

			spriteNumbers_[i]->SetTextureHandle(textureHandles_[result]);
		}
		break;
	default:
		break;
	}

	// 選択スプライトの位置を設定する
	spriteSelectItem_->SetPosition(spriteSelectItemPosition_);

	#ifdef _DEBUG

	ImGui::Begin("Option");
	ImGui::Text("Fov%f", camera_->GetFov());
	ImGui::Text("CameraSensitivityX%f", camera_->GetCameraSensitivity().x);
	ImGui::Text("CameraSensitivityY%f", camera_->GetCameraSensitivity().y);
	ImGui::End();

#endif // DEBUG

}

void OptionManager::Draw() {
	// 開かれている時のみ描画
	if (isOpen_) {
		spriteBackGround_->Draw(); // オプション背景
		spriteFOV_->Draw();        // FOV
		spriteFOVGageBackGround_->Draw(); // FOVゲージ背景
		spriteFOVGage_->Draw(); // FOVゲージ
		spriteCameraSensitivityX_->Draw(); // 水平カメラ感度
		spriteCameraSensitivityXGageBackGround_->Draw(); // 水平カメラ感度ゲージ背景
		spriteCameraSensitivityXGage_->Draw();	// 水平カメラ感度ゲージ
		spriteCameraSensitivityY_->Draw(); // 垂直カメラ感度
		spriteCameraSensitivityYGageBackGround_->Draw(); // 垂直カメラゲージ背景
		spriteCameraSensitivityYGage_->Draw();           // 垂直カメラゲージ
		spriteSelectItem_->Draw();         // 選択項目描画

		for (int i = 0; i < 3; i++) {
			spriteNumbers_[i]->Draw();
		}

	}
}

void OptionManager::OpenClose(_XINPUT_GAMEPAD gamePad, _XINPUT_GAMEPAD preGamePad) {

	//スタートボタンがおされたら切り替える
	if (gamePad.wButtons & XINPUT_GAMEPAD_START && 
			!(preGamePad.wButtons & XINPUT_GAMEPAD_START) || Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {

		//開くまたは閉じる
		if (isOpen_) {
			isOpen_ = false;
			// 閉じる音再生
			audio_->PlayWave(audioHandles_[Audio::close]);

		} else {
			// 開く
			audio_->PlayWave(audioHandles_[Audio::Open]);
			isOpen_ = true;
		}

	}

}
