#include "TutorialManager.h"
#include "WinApp.h"
#include "Enemy/EnemyManager.h"

TutorialManager::TutorialManager() {}

TutorialManager::~TutorialManager() {}

TutorialManager* TutorialManager::GetInstance() {
	// 関数内Static変数として宣言
	static TutorialManager instance;
	return &instance;
}

void TutorialManager::Initialize(
    const std::vector<Model*>& tutorialModels, const std::vector<uint32_t>& textureHandles,
    const std::vector<uint32_t>& audioHandles) {

    // モデル受け取り
	models_ = tutorialModels;
    // テクスチャ受け取り
	textureHandles_ = textureHandles;
    // 効果音受け取り
	audioHandles_ = audioHandles;

	// 入力取得
	input_ = Input::GetInstance();
	// ゲームパッドの状態取得
	input_->GetJoystickState(0, joyState);
	preJoyState = joyState;

	// 敵マネージャーのインスタンス取得
	enemyManager_ = EnemyManager::GetInstance();

	// 進捗リセット
	tutorialSteps_ = Move;

	// チュートリアル終了トリガーリセット
	isEndTutorial = false;

	// チュートリアル画像表示トリガーリセット
	displayTutorialImage_ = false;

	// チュートリアル進捗ゲージリセット
	tutorialGageProgress_ = 0;
	// 次のチュートリアルトリガーリセット
	isNextTutorial_ = false;

	// 最初の画像を読み込み
	textureHandleTutorialImage_ =
		TextureManager::Load("/Image/Tutorial/Image_1_OrbTutorialImageUI.png"); // オーブ

	// 最初のテキストを読み込み
	textureHandleTutorialText_ = 
		TextureManager::Load("/Image/Tutorial/1_MoveTutorialTextUI.png"); // 移動テキスト
	
	// チュートリアルゲージの大きさ
	tutorialGageSize_ = {500.0f, 16.0f};
	// チュートリアルゲージの座標
	tutorialGagePosition_ = {
	    (float)(WinApp::GetInstance()->kWindowWidth / 2) - tutorialGageSize_.x / 2.0f, 500.0f};
	// チュートリアルのゲージ背景スプライトリセット
	spriteTutorialGageBackGround_.reset(Sprite::Create(
	    textureHandles[0], {tutorialGagePosition_.x, tutorialGagePosition_.y},
	    {0.0f, 0.0f, 0.0f, 0.75f}, {0.0f, 0.5f}));
	spriteTutorialGageBackGround_->SetSize(tutorialGageSize_);
	// チュートリアルゲージスプライトリセット
	spriteTutorialGage_.reset(Sprite::Create(
	    textureHandles[0], {tutorialGagePosition_.x, tutorialGagePosition_.y},
	    {0.0f, 0.8f, 0.2f, 1.0f}, {0.0f, 0.5f}));
	spriteTutorialGage_->SetSize({0.0f, tutorialGageSize_.y});

	// チュートリアル画像のスプライト
	spriteTutorialImage_.reset(Sprite::Create(
	    textureHandleTutorialImage_, {0.0f, 0.0f},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.5f}));

	// チュートリアルテキストの大きさ
	tutorialTextSize_ = {832.0f, 96.0f};
	// チュートリアルテキスト座標
	tutorialTextPosition_ = {(float)WinApp::GetInstance()->kWindowWidth / 2, 600.0f};
	// チュートリアルテキスト背景のスプライト
	spriteTutorialTextBackGround_.reset(Sprite::Create(
	    textureHandles[0], {tutorialTextPosition_.x, tutorialTextPosition_.y},
	    {0.0f, 0.0f, 0.0f, 0.75f}, {0.5f, 0.5f}));
	spriteTutorialTextBackGround_->SetSize(
	    {tutorialTextSize_.x + 20.0f, tutorialTextSize_.y + 20.0f});
	// チュートリアルテキストのスプライト
	spriteTutorialText_.reset(Sprite::Create(
	    textureHandleTutorialText_, {tutorialTextPosition_.x, tutorialTextPosition_.y},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

}

void TutorialManager::Update() {

	// ゲームパッドの状態取得
	preJoyState = joyState;
	input_->GetJoystickState(0, joyState);

	// チュートリアル段階によって切り替え
	switch (tutorialSteps_) {
	case TutorialManager::Move:
		// 移動チュートリアル
		MoveTutorial();
		break;
	case TutorialManager::NormalShot:
		// 移動チュートリアル
		NormalShotTutorial();
		break;
	case TutorialManager::Orbs:

		break;
	case TutorialManager::FireBullet:

		break;
	case TutorialManager::IceBullet:

		break;
	case TutorialManager::ThunderBullet:

		break;
	case TutorialManager::OrbReinforcement:

		break;
	case TutorialManager::ChangeOrb:

		break;
	case TutorialManager::TutorialEnd:

		break;
	}

	// チュートリアルゲージ進捗が指定の値以上になったら制限する
	if (tutorialGageProgress_ > kMaxTutorialGage_) {
		tutorialGageProgress_ = kMaxTutorialGage_;
	}

	// チュートリアルゲージの更新
	float tutorialGageT = tutorialGageProgress_ / kMaxTutorialGage_;
	float gageLength = MyMath::Linear(tutorialGageT, 0.0f, tutorialGageSize_.x);
	spriteTutorialGage_->SetSize({gageLength, spriteTutorialGage_->GetSize().y});

	// チュートリアルゲージ進捗が完全になったら次のチュートリアルへ進む
	if (tutorialGageProgress_ == kMaxTutorialGage_) {
		isNextTutorial_ = true;
	}

	// チュートリアル終了が終了段階なら
	if (tutorialSteps_ == TutorialEnd) {
		// Bボタンが押されたら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
		    preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			// チュートリアル終了
			isEndTutorial = true;
		}
	}

	/// UI関連の更新
	// チュートリアル進捗ゲージ背景
	spriteTutorialGageBackGround_->SetSize(tutorialGageSize_);         // 大きさ
	spriteTutorialGageBackGround_->SetPosition(tutorialGagePosition_); // 座標
	// チュートリアル進捗ゲージ
	spriteTutorialGage_->SetPosition(tutorialGagePosition_); // 座標
	// チュートリアルテキスト背景
	spriteTutorialTextBackGround_->SetSize(
	    {tutorialTextSize_.x + 20.0f, tutorialTextSize_.y + 20.0f});   // 大きさ
	spriteTutorialTextBackGround_->SetPosition(tutorialTextPosition_); // 座標
	// チュートリアルテキスト
	spriteTutorialText_->SetSize(tutorialTextSize_);         // 大きさ
	spriteTutorialText_->SetPosition(tutorialTextPosition_); // 座標
}

void TutorialManager::SpriteDraw() {
	// チュートリアルゲージ背景
	spriteTutorialGageBackGround_->Draw();
	// チュートリアルゲージ
	spriteTutorialGage_->Draw();

	// スプライトの画像表示トリガーがオンなら
	if (displayTutorialImage_) {
		// チュートリアル画像を描画
		spriteTutorialImage_->Draw();
	}

	// チュートリアルテキスト背景
	spriteTutorialTextBackGround_->Draw();
	// チュートリアルテキスト
	spriteTutorialText_->Draw();
}

void TutorialManager::MoveTutorial() {
	// ジョイスティックの状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		// スティックの入力に応じて移動
		Vector3 stickVec = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX,
		    -(float)joyState.Gamepad.sThumbLY / SHRT_MAX, 0.0f};

		// 正規化
		stickVec = MyMath::Normalize(stickVec);
		// ベクトルの長さを求める
		float stickVecLength = MyMath::Length(stickVec);
		// ベクトルの長さを少し短く
		stickVecLength /= 6.0f;

		if (stickVecLength < 0) {
			stickVecLength *= -1.0f;
		}

		// 進捗にプラスする
		tutorialGageProgress_ += stickVecLength;

		// ジャンプすると
		if (player_->GetIsGround()) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
			    !(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				// チュートリアルゲージを進める
				tutorialGageProgress_ += 10.0f;
			}
		}
	}

	// 次のチュートリアルへ進む場合
	if (isNextTutorial_) {
		// テクスチャを一度アンロード
		TextureManager::Unload(textureHandleTutorialText_);
		// テクスチャ再度読み込み
		textureHandleTutorialText_ =
		    TextureManager::Load("/Image/Tutorial/2_NormalShotTotorialTextUI.png");

		// ゲージ進捗リセット
		tutorialGageProgress_ = 0.0f;
		// 次のチュートリアルへ
		tutorialSteps_++;

		// トリガーリセット
		isNextTutorial_ = false;
	}
}

void TutorialManager::NormalShotTutorial() {
	// Rトリガーが押されたら
	if (joyState.Gamepad.bRightTrigger > 25) {
		// 進捗にプラスする
		tutorialGageProgress_ += 0.35f;
	}

	// 次のチュートリアルへ進む場合
	if (isNextTutorial_) {
		// テクスチャを一度アンロード
		TextureManager::Unload(textureHandleTutorialText_);
		// テクスチャ再度読み込み
		textureHandleTutorialText_ =
		    TextureManager::Load("/Image/Tutorial/3_NormalShotTotorialTextUI.png");

		// チュートリアル用敵の出現
		enemyManager_->AddEnemy({0.0f, 25.0f, 0.0f}, Enemy::Fire, true);

		// ゲージ進捗リセット
		tutorialGageProgress_ = 0.0f;
		// 次のチュートリアルへ
		tutorialSteps_++;

		// トリガーリセット
		isNextTutorial_ = false;
	}
}
