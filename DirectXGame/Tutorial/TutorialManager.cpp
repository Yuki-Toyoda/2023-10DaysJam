#include "TutorialManager.h"
#include "WinApp.h"

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

	// 進捗リセット
	tutorialSteps_ = Move;

	// チュートリアル終了トリガーリセット
	isEndTutorial = false;

	// チュートリアル画像表示トリガーリセット
	displayTutorialImage_ = false;

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
	XINPUT_STATE joyState;
	if (input_->GetJoystickState(0, joyState)) {
		// チュートリアル段階によって切り替え
		switch (tutorialSteps_) {
		case TutorialManager::Move:

			break;
		case TutorialManager::NormalShot:

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
	}
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
