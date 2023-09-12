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

	// 音検知
	audio_ = Audio::GetInstance();

	// 敵マネージャーのインスタンス取得
	enemyManager_ = EnemyManager::GetInstance();

	// 進捗リセット
	tutorialSteps_ = Move;
	// 個々のチュートリアル演出中間地点リセット
	tutorialStagingWayPoint_ = WayPoint1;
	// チュートリアル演出用t
	tutorialT_ = 0.0f;

	// チュートリアルスキップ演出用t
	tutorialSkipT_ = 0.0f;

	// チュートリアル終了トリガーリセット
	isEndTutorial = false;

	// チュートリアル画像表示トリガーリセット
	displayTutorialImage_ = false;

	// チュートリアル進捗ゲージリセット
	tutorialGageProgress_ = 0;
	// 次のチュートリアルトリガーリセット
	isNextTutorial_ = false;

	// チュートリアル画像の表示切り替えのトリガー
	tutorialImageChangeTrigger_ = false;
	tutorialImagewayPoint_ = 0;

	enemySpawnInterval_ = 0.0f;

	// 最初の画像を読み込み
	textureHandleTutorialImage_ =
		TextureManager::Load("/Image/Tutorial/Image_1_OrbTutorialImageUI.png"); // オーブ

	// 最初のテキストを読み込み
	textureHandleTutorialText_ = 
		TextureManager::Load("/Image/Tutorial/1_MoveTutorialTextUI.png"); // 移動テキスト
	// 次のチュートリアルへ促すテクスチャ
	textureHandleNextTutorial_ =
	    TextureManager::Load("/Image/Tutorial/NextTutorialText.png");
	// チュートリアルスキップテクスチャ
	textureHandleSkipTutorial_ = TextureManager::Load("/Image/Tutorial/tutorialSkipUI.png");

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
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}));

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

	// 次のチュートリアルへ促すスプライト
	spriteNextTutorial_.reset(Sprite::Create(
	    textureHandleNextTutorial_,
	    {tutorialTextPosition_.x + 300.0f, tutorialTextPosition_.y - 250.0f},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	// チュートリアルスキップテキストの座標
	tutorialSkipTextPosition_ = {300.0f, 50.0f};
	// チュートリアルスキップテキストの大きさ
	tutorialSkipTextSize_ = {363.0f, 64.0f};
	// チュートリアルスキップテキスト
	spriteTutorialSkipText_.reset(Sprite::Create(
	    textureHandleSkipTutorial_, tutorialSkipTextPosition_, {1.0f, 1.0f, 1.0f, 1.0f},
	    {0.5f, 0.5f}));
	// チュートリアルスキップテキスト用進捗ゲージスプライト
	spriteTutorialGageSkipText_.reset(Sprite::Create(
	    textureHandles_[0],
	    {tutorialSkipTextPosition_.x + tutorialSkipTextSize_.x / 2.25f, tutorialSkipTextPosition_.y}, 
		{1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.5f}));
	spriteTutorialGageSkipText_->SetSize({0.0f, tutorialSkipTextSize_.y});

}

void TutorialManager::SetUp() {

	// 進捗リセット
	tutorialSteps_ = Move;
	// 個々のチュートリアル演出中間地点リセット
	tutorialStagingWayPoint_ = WayPoint1;
	// チュートリアル演出用t
	tutorialT_ = 0.0f;

	// チュートリアルスキップ演出用t
	tutorialSkipT_ = 0.0f;
	// チュートリアルスキップにかかる秒数
	tutorialSkipTime_ = 1.25f;

	// チュートリアル終了トリガーリセット
	isEndTutorial = false;

	// チュートリアル画像表示トリガーリセット
	displayTutorialImage_ = false;

	// チュートリアル進捗ゲージリセット
	tutorialGageProgress_ = 0;
	// 次のチュートリアルトリガーリセット
	isNextTutorial_ = false;

	// チュートリアル画像の表示切り替えのトリガー
	tutorialImageChangeTrigger_ = false;
	tutorialImagewayPoint_ = 0;

	// スポーン間隔リセット
	enemySpawnInterval_ = 0.0f;

	// チュートリアルゲージの大きさ
	tutorialGageSize_ = {500.0f, 16.0f};
	// チュートリアルゲージの座標
	tutorialGagePosition_ = {
	    (float)(WinApp::GetInstance()->kWindowWidth / 2) - tutorialGageSize_.x / 2.0f, 500.0f};

	// チュートリアルテキストの大きさ
	tutorialTextSize_ = {832.0f, 96.0f};
	// チュートリアルテキスト座標
	tutorialTextPosition_ = {(float)WinApp::GetInstance()->kWindowWidth / 2, 600.0f};

	// チュートリアルスキップテキストの座標
	tutorialSkipTextPosition_ = {(float)WinApp::GetInstance()->kWindowWidth / 2, 50.0f};
	// チュートリアルスキップテキストの大きさ
	tutorialSkipTextSize_ = {363.0f, 64.0f};

	// チュートリアルスキップテキスト用進捗ゲージスプライト
	spriteTutorialGageSkipText_->SetSize({0.0f, tutorialSkipTextSize_.y});

	// 最初の画像を読み込み
	TextureManager::Unload(textureHandleTutorialImage_); // 一応アンロード
	textureHandleTutorialImage_ =
	    TextureManager::Load("/Image/Tutorial/Image_1_OrbTutorialImageUI.png"); // オーブ

	// 最初のテキストを読み込み
	TextureManager::Unload(textureHandleTutorialText_); // 一応アンロード
	textureHandleTutorialText_ =
	    TextureManager::Load("/Image/Tutorial/1_MoveTutorialTextUI.png"); // 移動テキスト

	// プレイヤーの根本トリガーをセット
	player_->SetCanAction(true); // 行動可能に
	player_->SetRootCanMove(true); // 移動は可能
	player_->SetRootCanShot(false); // 通常射撃を不可能に
	player_->SetRootCanSpecialShot(false); // 特殊射撃を不可能に
	player_->SetRootCanSelectOrb(false);   // オーブ選択を不可能に
	player_->SetRootCanChangeOrbColor(false); // オーブ色選択を不可能に
	player_->SetRootCanChangeOrb(false);     // オーブ変換を不可能に

}

void TutorialManager::Update() {

	spriteNextTutorial_->SetPosition(
	    {tutorialTextPosition_.x + 450.0f, tutorialTextPosition_.y - 175.0f});

	// ゲームパッドの状態取得
	preJoyState = joyState;
	input_->GetJoystickState(0, joyState);

	// Yボタンが押されると
	if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) && !isEndTutorial) {
		if (tutorialSkipT_ >= tutorialSkipTime_) {
			// チュートリアルを終了させる
			tutorialSteps_ = TutorialEnd;
		} else {
			// 押している間加算
			tutorialSkipT_ += 1.0f / 60.0f;
		}
	} else {
		if (tutorialSkipT_ > 0) {
			// 押していない間は減算する
			tutorialSkipT_ -= 1.0f / 60.0f;
		} else {
			// 0いかにならないよう固定
			tutorialSkipT_ = 0.0f;
		}
	}

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
		// オーブチュートリアル
		OrbTutorial();
		break;
	case TutorialManager::FireBullet:
		// 炎弾チュートリアル
		FireBulletTutorial();
		break;
	case TutorialManager::IceBullet:
		// 氷弾チュートリアル
		IceBulletTutorial();
		break;
	case TutorialManager::ThunderBullet:
		// 雷弾チュートリアル
		ThunderBulletTutorial();
		break;
	case TutorialManager::OrbReinforcement:
		// オーブ強化チュートリアル
		OrbReinforcementTutorial();
		break;
	case TutorialManager::ChangeOrb:
		// オーブ変換チュートリアル
		ChangeOrbTutorial();
		break;
	case TutorialManager::TutorialEnd:

		// 敵がわいていないならランダムに沸かせる
		if ((int)enemyManager_->GetEnemyCount() < 1 && enemySpawnInterval_ <= 0) {
			int enemyType = MyMath::Random(Enemy::Fire, Enemy::Thunder);
			// チュートリアル用敵の出現
			enemyManager_->AddEnemy({0.0f, 25.0f, 0.0f}, (Enemy::EnemyType)enemyType, true);
			// 敵スポーン間隔リセット
			enemySpawnInterval_ = 1.0f;
		}

		// 敵スポーン間隔が0以下になる前に
		if (enemySpawnInterval_ > 0 && (int)enemyManager_->GetEnemyCount() <= 0) {
			// 敵スポーン間隔をデクリメント
			enemySpawnInterval_ -= 1.0f / 60.0f;
		}

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
		// Bボタンが押されたらまたはチュートリアルスキップを行うなら
		if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
		    !(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B)) || 
			tutorialSkipT_ >= tutorialSkipTime_) {
			// チュートリアルスキップ用tを初期化
			tutorialSkipT_ = 0.0f;
			// 次へ音再生
			audio_->PlayWave(audioHandles_[Audio::endTutorial]);

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
	// チュートリアルスキップテキスト
	spriteTutorialSkipText_->SetSize(tutorialSkipTextSize_); // 大きさ
	spriteTutorialSkipText_->SetPosition(tutorialSkipTextPosition_); // 位置
	float skipGageLength =
	    MyMath::Linear(tutorialSkipT_, 0.0f, tutorialSkipTextSize_.x, tutorialSkipTime_);
	spriteTutorialGageSkipText_->SetSize({skipGageLength, tutorialSkipTextSize_.y});

	// チュートリアル中はHHPが位置を下回らないように
	if (player_->GetHp() <= 0) {
		player_->SetHP(1);
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
		spriteNextTutorial_->Draw();
	}

	// チュートリアルテキスト背景
	spriteTutorialTextBackGround_->Draw();
	// チュートリアルテキスト
	spriteTutorialText_->Draw();

	// チュートリアルスキップテキスト
	spriteTutorialSkipText_->Draw();
	spriteTutorialGageSkipText_->Draw();
}

void TutorialManager::MoveTutorial() {
	// スティックの入力に応じて移動
	Vector3 stickVec = {
	    (float)joyState.Gamepad.sThumbLX / SHRT_MAX, -(float)joyState.Gamepad.sThumbLY / SHRT_MAX,
	    0.0f};

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

	// 次のチュートリアルへ進む場合
	if (isNextTutorial_) {
		// テクスチャを一度アンロード
		TextureManager::Unload(textureHandleTutorialText_);
		// テクスチャ再度読み込み
		textureHandleTutorialText_ =
		    TextureManager::Load("/Image/Tutorial/2_NormalShotTotorialTextUI.png");

		// プレイヤーの通常射撃を可能に
		player_->SetRootCanShot(true);

		// トリガーリセット
		isNextTutorial_ = false;

		// クリア音再生
		audio_->PlayWave(audioHandles_[Audio::clearTutorial]);

		// ゲージ進捗リセット
		tutorialGageProgress_ = 0.0f;
		// 次のチュートリアルへ
		tutorialSteps_++;
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

		// クリア音再生
		audio_->PlayWave(audioHandles_[Audio::clearTutorial]);

		// ゲージ進捗リセット
		tutorialGageProgress_ = 0.0f;
		// 次のチュートリアルへ
		tutorialSteps_++;

		// トリガーリセット
		isNextTutorial_ = false;
	}
}

void TutorialManager::OrbTutorial() {
	switch (tutorialStagingWayPoint_) {
	case TutorialManager::WayPoint1:
		// プレイヤーのオーブ所持数が0を超えた場合
		if (player_->GetHavingOrbCount() > 0) {
			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialText_);
			// テクスチャ再度読み込み
			textureHandleTutorialText_ =
			    TextureManager::Load("/Image/Tutorial/4_OrbTutorialTextUI.png");
			// チュートリアルのテキスト大きさ画像に合わせて再設定
			tutorialTextSize_ = {832.0f, 192.0f};
			tutorialGagePosition_.y = 475.0f;

			// プレイヤーを動けない状態に
			player_->SetCanAction(false);

			// クリア音再生
			audio_->PlayWave(audioHandles_[Audio::clearTutorial]);

			// チュートリアル用画像表示
			displayTutorialImage_ = true;
			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;
		}
		break;
	case TutorialManager::WayPoint2:
		// Bボタンが押されたら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
		    !(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {
			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialText_);
			// テクスチャ再度読み込み
			textureHandleTutorialText_ =
			    TextureManager::Load("/Image/Tutorial/5_OrbTutorialTextUI.png");

			// プレイヤーを動ける状態に
			player_->SetCanAction(true);
			// プレイヤーの特殊射撃をできるように
			player_->SetRootCanSpecialShot(true);

			// チュートリアル用画像非表示
			displayTutorialImage_ = false;

			// 次へ音再生
			audio_->PlayWave(audioHandles_[Audio::NextTutorial]);

			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;
		}
		break;
	case TutorialManager::WayPoint3:
		// Lトリガーが押されたら
		if (joyState.Gamepad.bLeftTrigger > 25) {
			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;

			// クリア音再生
			audio_->PlayWave(audioHandles_[Audio::clearTutorial]);

		}
		break;
	case TutorialManager::WayPoint4:
		// 指定秒数経つと
		if (tutorialT_ < 2.0f) {
			// 演出用t加算
			tutorialT_ += 1.0f / 60.0f;
		} else {
			// 演出用tリセット
			tutorialT_ = 0.0f;

			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialText_);
			// テクスチャ再度読み込み
			textureHandleTutorialText_ =
			    TextureManager::Load("/Image/Tutorial/SpecialShotTutorial.png");

			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;

		}
		break;
	}

	// チュートリアル演出が終端なら
	if (tutorialStagingWayPoint_ == WayPoint5) {
		// 進捗を進める
		tutorialGageProgress_ += 0.45f;
	}

	// 次のチュートリアルへ進む場合
	if (isNextTutorial_) {
		// テクスチャを一度アンロード
		TextureManager::Unload(textureHandleTutorialText_);
		// テクスチャ再度読み込み
		textureHandleTutorialText_ =
		    TextureManager::Load("/Image/Tutorial/6_FireBulletTutorialTextUI.png");

		// テクスチャを一度アンロード
		TextureManager::Unload(textureHandleTutorialImage_);
		// テクスチャ再度読み込み
		textureHandleTutorialImage_ =
		    TextureManager::Load("/Image/Tutorial/Image_2_SpecialShotTutorialImageUI.png");
		// チュートリアル用画像表示
		displayTutorialImage_ = true;

		// プレイヤーを動けない状態に
		player_->SetCanAction(false);

		// チュートリアル個々進捗リセット
		tutorialStagingWayPoint_ = WayPoint1;

		// ゲージ進捗リセット
		tutorialGageProgress_ = 0.0f;
		// 次のチュートリアルへ
		tutorialSteps_++;

		// トリガーリセット
		isNextTutorial_ = false;
	}
}

void TutorialManager::FireBulletTutorial() {
	switch (tutorialStagingWayPoint_) {
	case TutorialManager::WayPoint1:
		// チュートリアルの画像切り替え
		switch (tutorialImagewayPoint_) {
		case 0:
			if (tutorialT_ < 0.5f) {
				// 演出用t加算
				tutorialT_ += 1.0f / 60.0f;
			} else {
				// テクスチャを一度アンロード
				TextureManager::Unload(textureHandleTutorialImage_);
				// テクスチャ再度読み込み
				textureHandleTutorialImage_ =
				    TextureManager::Load("/Image/Tutorial/Image_3_SpecialShotTutorialImageUI.png");

				// チュートリアル演出tリセット
				tutorialT_ = 0.0f;
				// チュートリアル画像を切り替え
				tutorialImagewayPoint_++;
			}
			break;
			case 1:
			if (tutorialT_ < 0.5f) {
				// 演出用t加算
				tutorialT_ += 1.0f / 60.0f;
			} else {
				// テクスチャを一度アンロード
				TextureManager::Unload(textureHandleTutorialImage_);
				// テクスチャ再度読み込み
				textureHandleTutorialImage_ =
				    TextureManager::Load("/Image/Tutorial/Image_4_FireBulletTutorialImageUI.png");

				// チュートリアル演出tリセット
				tutorialT_ = 0.0f;
				// チュートリアル画像を切り替え
				tutorialImagewayPoint_++;
			}
			break;
			case 2:
			if (tutorialT_ < 1.0f) {
				// 演出用t加算
				tutorialT_ += 1.0f / 60.0f;
			} else {
				// テクスチャを一度アンロード
				TextureManager::Unload(textureHandleTutorialImage_);
				// テクスチャ再度読み込み
				textureHandleTutorialImage_ =
				    TextureManager::Load("/Image/Tutorial/Image_2_SpecialShotTutorialImageUI.png");

				// チュートリアル演出tリセット
				tutorialT_ = 0.0f;
				// チュートリアル画像を切り替え
				tutorialImagewayPoint_ = 0;
			}
			break;

		}

		// Bボタンが押されたら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
		    !(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {
			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialText_);
			// テクスチャ再度読み込み
			textureHandleTutorialText_ =
			    TextureManager::Load("/Image/Tutorial/GetOrbTutorialText.png");

			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialImage_);
			// テクスチャ再度読み込み
			textureHandleTutorialImage_ =
			    TextureManager::Load("/Image/Tutorial/Image_2_SpecialShotTutorialImageUI.png");

			// チュートリアル用画像非表示
			displayTutorialImage_ = false;

			// プレイヤーを動ける状態に
			player_->SetCanAction(true);

			// 次へ音再生
			audio_->PlayWave(audioHandles_[Audio::NextTutorial]);

			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;
		}
		break;
	case TutorialManager::WayPoint2:
		// プレイヤーのオーブ所持数が0を超えたら
		if (player_->GetHavingOrbCount() > 0) {
			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialText_);
			// テクスチャ再度読み込み
			textureHandleTutorialText_ =
			    TextureManager::Load("/Image/Tutorial/5_OrbTutorialTextUI.png");

			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;
		}
		break;
	case TutorialManager::WayPoint3:
		// Lトリガーが押されたら
		if (joyState.Gamepad.bLeftTrigger > 25) {

			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialText_);
			// テクスチャ再度読み込み
			textureHandleTutorialText_ =
			    TextureManager::Load("/Image/Tutorial/NextTutorialUIText.png");
			// チュートリアルのテキスト大きさ画像に合わせて再設定
			tutorialTextSize_ = {832.0f, 96.0f};
			tutorialGagePosition_.y = 600.0f;

			// クリア音再生
			audio_->PlayWave(audioHandles_[Audio::clearTutorial]);

			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;
		}
		break;
	case TutorialManager::WayPoint4:

		// Bボタンが押されたら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
		    !(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {

			// 次へ音再生
			audio_->PlayWave(audioHandles_[Audio::NextTutorial]);

			// 次のチュートリアルへ
			isNextTutorial_ = true;
		}

		break;
	}

	// 敵がわいていないなら沸かせる
	if ((int)enemyManager_->GetEnemyCount() < 1 && enemySpawnInterval_ <= 0) {
		// チュートリアル用敵の出現
		enemyManager_->AddEnemy({0.0f, 25.0f, 0.0f}, Enemy::Fire, true);
		// 敵スポーン間隔リセット
		enemySpawnInterval_ = 1.0f;
	}

	// 敵スポーン間隔が0以下になる前に
	if (enemySpawnInterval_ > 0 && (int)enemyManager_->GetEnemyCount() <= 0) {
		// 敵スポーン間隔をデクリメント
		enemySpawnInterval_ -= 1.0f / 60.0f;
	}

	// 次のチュートリアルへ進む場合
	if (isNextTutorial_) {
		// テクスチャを一度アンロード
		TextureManager::Unload(textureHandleTutorialText_);
		// テクスチャ再度読み込み
		textureHandleTutorialText_ =
		    TextureManager::Load("/Image/Tutorial/7_IceBulletTutorialTextUI.png");
		// チュートリアルのテキスト大きさ画像に合わせて再設定
		tutorialTextSize_ = {832.0f, 192.0f};
		tutorialGagePosition_.y = 475.0f;

		// 画像中間地点リセット
		tutorialImagewayPoint_ = 0;
		// テクスチャを一度アンロード
		TextureManager::Unload(textureHandleTutorialImage_);
		// テクスチャ再度読み込み
		textureHandleTutorialImage_ =
		    TextureManager::Load("/Image/Tutorial/Image_2_SpecialShotTutorialImageUI.png");
		// チュートリアル用画像表示
		displayTutorialImage_ = true;

		// プレイヤーを動けない状態に
		player_->SetCanAction(false);

		// チュートリアル個々進捗リセット
		tutorialStagingWayPoint_ = WayPoint1;

		// プレイヤーの所持オーブリセット
		player_->ResetOrbs();

		// 敵を全て削除
		enemyManager_->Delete();
		enemyManager_->SetEnemyCount(0);

		// ゲージ進捗リセット
		tutorialGageProgress_ = 0.0f;
		// 次のチュートリアルへ
		tutorialSteps_++;

		// トリガーリセット
		isNextTutorial_ = false;
	}
}

void TutorialManager::IceBulletTutorial() {
	switch (tutorialStagingWayPoint_) {
	case TutorialManager::WayPoint1:
		// チュートリアルの画像切り替え
		switch (tutorialImagewayPoint_) {
		case 0:
			if (tutorialT_ < 0.5f) {
				// 演出用t加算
				tutorialT_ += 1.0f / 60.0f;
			} else {
				// テクスチャを一度アンロード
				TextureManager::Unload(textureHandleTutorialImage_);
				// テクスチャ再度読み込み
				textureHandleTutorialImage_ =
				    TextureManager::Load("/Image/Tutorial/Image_3_SpecialShotTutorialImageUI.png");

				// チュートリアル演出tリセット
				tutorialT_ = 0.0f;
				// チュートリアル画像を切り替え
				tutorialImagewayPoint_++;
			}
			break;
		case 1:
			if (tutorialT_ < 0.5f) {
				// 演出用t加算
				tutorialT_ += 1.0f / 60.0f;
			} else {
				// テクスチャを一度アンロード
				TextureManager::Unload(textureHandleTutorialImage_);
				// テクスチャ再度読み込み
				textureHandleTutorialImage_ =
				    TextureManager::Load("/Image/Tutorial/Image_5_IceBulletTutorialImageUI.png");

				// チュートリアル演出tリセット
				tutorialT_ = 0.0f;
				// チュートリアル画像を切り替え
				tutorialImagewayPoint_++;
			}
			break;
		case 2:
			if (tutorialT_ < 1.0f) {
				// 演出用t加算
				tutorialT_ += 1.0f / 60.0f;
			} else {
				// テクスチャを一度アンロード
				TextureManager::Unload(textureHandleTutorialImage_);
				// テクスチャ再度読み込み
				textureHandleTutorialImage_ =
				    TextureManager::Load("/Image/Tutorial/Image_2_SpecialShotTutorialImageUI.png");

				// チュートリアル演出tリセット
				tutorialT_ = 0.0f;
				// チュートリアル画像を切り替え
				tutorialImagewayPoint_ = 0;
			}
			break;
		}

		// Bボタンが押されたら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
		    !(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {
			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialText_);
			// テクスチャ再度読み込み
			textureHandleTutorialText_ =
			    TextureManager::Load("/Image/Tutorial/GetOrbTutorialText.png");

			// 画像中間地点リセット
			tutorialImagewayPoint_ = 0;
			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialImage_);
			// テクスチャ再度読み込み
			textureHandleTutorialImage_ =
			    TextureManager::Load("/Image/Tutorial/Image_2_SpecialShotTutorialImageUI.png");

			// チュートリアル用画像非表示
			displayTutorialImage_ = false;

			// プレイヤーを動ける状態に
			player_->SetCanAction(true);

			// 次へ音再生
			audio_->PlayWave(audioHandles_[Audio::NextTutorial]);

			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;
		}
		break;
	case TutorialManager::WayPoint2:
		// プレイヤーのオーブ所持数が0を超えたら
		if (player_->GetHavingOrbCount() > 0) {
			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialText_);
			// テクスチャ再度読み込み
			textureHandleTutorialText_ =
			    TextureManager::Load("/Image/Tutorial/5_OrbTutorialTextUI.png");

			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;
		}
		break;
	case TutorialManager::WayPoint3:
		// Lトリガーが押されたら
		if (joyState.Gamepad.bLeftTrigger > 25) {

			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialText_);
			// テクスチャ再度読み込み
			textureHandleTutorialText_ =
			    TextureManager::Load("/Image/Tutorial/NextTutorialUIText.png");
			// チュートリアルのテキスト大きさ画像に合わせて再設定
			tutorialTextSize_ = {832.0f, 96.0f};
			tutorialGagePosition_.y = 600.0f;

			// クリア音再生
			audio_->PlayWave(audioHandles_[Audio::clearTutorial]);

			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;
		}
		break;
	case TutorialManager::WayPoint4:

		// Bボタンが押されたら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
		    !(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {

			// 次へ音再生
			audio_->PlayWave(audioHandles_[Audio::NextTutorial]);

			// 次のチュートリアルへ
			isNextTutorial_ = true;
		}

		break;
	}

	// 敵がわいていないなら沸かせる
	if ((int)enemyManager_->GetEnemyCount() < 1 && enemySpawnInterval_ <= 0) {
		// チュートリアル用敵の出現
		enemyManager_->AddEnemy({0.0f, 25.0f, 0.0f}, Enemy::Ice, true);
		// 敵スポーン間隔リセット
		enemySpawnInterval_ = 1.0f;
	}

	// 敵スポーン間隔が0以下になる前に
	if (enemySpawnInterval_ > 0 && (int)enemyManager_->GetEnemyCount() <= 0) {
		// 敵スポーン間隔をデクリメント
		enemySpawnInterval_ -= 1.0f / 60.0f;
	}

	// 次のチュートリアルへ進む場合
	if (isNextTutorial_) {
		// テクスチャを一度アンロード
		TextureManager::Unload(textureHandleTutorialText_);
		// テクスチャ再度読み込み
		textureHandleTutorialText_ =
		    TextureManager::Load("/Image/Tutorial/8_ThunderBulletTutorialTextUI.png");
		// チュートリアルのテキスト大きさ画像に合わせて再設定
		tutorialTextSize_ = {832.0f, 192.0f};
		tutorialGagePosition_.y = 475.0f;

		// テクスチャを一度アンロード
		TextureManager::Unload(textureHandleTutorialImage_);
		// テクスチャ再度読み込み
		textureHandleTutorialImage_ =
		    TextureManager::Load("/Image/Tutorial/Image_2_SpecialShotTutorialImageUI.png");
		// チュートリアル用画像表示
		displayTutorialImage_ = true;

		// プレイヤーを動けない状態に
		player_->SetCanAction(false);

		// チュートリアル個々進捗リセット
		tutorialStagingWayPoint_ = WayPoint1;

		// プレイヤーの所持オーブリセット
		player_->ResetOrbs();

		// 敵を全て削除
		enemyManager_->Delete();
		enemyManager_->SetEnemyCount(0);

		// ゲージ進捗リセット
		tutorialGageProgress_ = 0.0f;
		// 次のチュートリアルへ
		tutorialSteps_++;

		// トリガーリセット
		isNextTutorial_ = false;
	}
}

void TutorialManager::ThunderBulletTutorial() {
	switch (tutorialStagingWayPoint_) {
	case TutorialManager::WayPoint1:
		// チュートリアルの画像切り替え
		switch (tutorialImagewayPoint_) {
		case 0:
			if (tutorialT_ < 0.5f) {
				// 演出用t加算
				tutorialT_ += 1.0f / 60.0f;
			} else {
				// テクスチャを一度アンロード
				TextureManager::Unload(textureHandleTutorialImage_);
				// テクスチャ再度読み込み
				textureHandleTutorialImage_ =
				    TextureManager::Load("/Image/Tutorial/Image_3_SpecialShotTutorialImageUI.png");

				// チュートリアル演出tリセット
				tutorialT_ = 0.0f;
				// チュートリアル画像を切り替え
				tutorialImagewayPoint_++;
			}
			break;
		case 1:
			if (tutorialT_ < 0.5f) {
				// 演出用t加算
				tutorialT_ += 1.0f / 60.0f;
			} else {
				// テクスチャを一度アンロード
				TextureManager::Unload(textureHandleTutorialImage_);
				// テクスチャ再度読み込み
				textureHandleTutorialImage_ =
				    TextureManager::Load("/Image/Tutorial/Image_6_ThunderBulletTutorialImageUI.png");

				// チュートリアル演出tリセット
				tutorialT_ = 0.0f;
				// チュートリアル画像を切り替え
				tutorialImagewayPoint_++;
			}
			break;
		case 2:
			if (tutorialT_ < 1.0f) {
				// 演出用t加算
				tutorialT_ += 1.0f / 60.0f;
			} else {
				// テクスチャを一度アンロード
				TextureManager::Unload(textureHandleTutorialImage_);
				// テクスチャ再度読み込み
				textureHandleTutorialImage_ =
				    TextureManager::Load("/Image/Tutorial/Image_2_SpecialShotTutorialImageUI.png");

				// チュートリアル演出tリセット
				tutorialT_ = 0.0f;
				// チュートリアル画像を切り替え
				tutorialImagewayPoint_ = 0;
			}
			break;
		}

		// Bボタンが押されたら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
		    !(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {
			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialText_);
			// テクスチャ再度読み込み
			textureHandleTutorialText_ =
			    TextureManager::Load("/Image/Tutorial/GetOrbTutorialText.png");

			// 画像中間地点リセット
			tutorialImagewayPoint_ = 0;
			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialImage_);
			// テクスチャ再度読み込み
			textureHandleTutorialImage_ =
			    TextureManager::Load("/Image/Tutorial/Image_2_SpecialShotTutorialImageUI.png");

			// チュートリアル用画像非表示
			displayTutorialImage_ = false;

			// プレイヤーを動ける状態に
			player_->SetCanAction(true);

			// 次へ音再生
			audio_->PlayWave(audioHandles_[Audio::NextTutorial]);

			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;
		}
		break;
	case TutorialManager::WayPoint2:
		// プレイヤーのオーブ所持数が0を超えたら
		if (player_->GetHavingOrbCount() > 0) {
			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialText_);
			// テクスチャ再度読み込み
			textureHandleTutorialText_ =
			    TextureManager::Load("/Image/Tutorial/5_OrbTutorialTextUI.png");

			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;
		}
		break;
	case TutorialManager::WayPoint3:
		// Lトリガーが押されたら
		if (joyState.Gamepad.bLeftTrigger > 25) {

			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialText_);
			// テクスチャ再度読み込み
			textureHandleTutorialText_ =
			    TextureManager::Load("/Image/Tutorial/NextTutorialUIText.png");
			// チュートリアルのテキスト大きさ画像に合わせて再設定
			tutorialTextSize_ = {832.0f, 96.0f};
			tutorialGagePosition_.y = 600.0f;

			// クリア音再生
			audio_->PlayWave(audioHandles_[Audio::clearTutorial]);

			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;
		}
		break;
	case TutorialManager::WayPoint4:

		// Bボタンが押されたら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
		    !(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {

			// 次へ音再生
			audio_->PlayWave(audioHandles_[Audio::NextTutorial]);

			// 次のチュートリアルへ
			isNextTutorial_ = true;
		}

		break;
	}

	// 敵がわいていないなら沸かせる
	if ((int)enemyManager_->GetEnemyCount() < 1 && enemySpawnInterval_ <= 0) {
		// チュートリアル用敵の出現
		enemyManager_->AddEnemy({0.0f, 25.0f, 0.0f}, Enemy::Thunder, true);
		// 敵スポーン間隔リセット
		enemySpawnInterval_ = 1.0f;
	}

	// 敵スポーン間隔が0以下になる前に
	if (enemySpawnInterval_ > 0 && (int)enemyManager_->GetEnemyCount() <= 0) {
		// 敵スポーン間隔をデクリメント
		enemySpawnInterval_ -= 1.0f / 60.0f;
	}

	// 次のチュートリアルへ進む場合
	if (isNextTutorial_) {
		// テクスチャを一度アンロード
		TextureManager::Unload(textureHandleTutorialText_);
		// テクスチャ再度読み込み
		textureHandleTutorialText_ =
		    TextureManager::Load("/Image/Tutorial/9_OrbReinforcementTutorialTextUI.png");
		// チュートリアルのテキスト大きさ画像に合わせて再設定
		tutorialTextSize_ = {832.0f, 192.0f};
		tutorialGagePosition_.y = 475.0f;

		// テクスチャを一度アンロード
		TextureManager::Unload(textureHandleTutorialImage_);
		// テクスチャ再度読み込み
		textureHandleTutorialImage_ =
		    TextureManager::Load("/Image/Tutorial/Image_1_OrbTutorialImageUI.png");
		// チュートリアル用画像表示
		displayTutorialImage_ = true;

		// プレイヤーを動けない状態に
		player_->SetCanAction(false);

		// チュートリアル個々進捗リセット
		tutorialStagingWayPoint_ = WayPoint1;

		// プレイヤーの所持オーブリセット
		player_->ResetOrbs();

		// 敵を全て削除
		enemyManager_->Delete();
		enemyManager_->SetEnemyCount(0);

		// ゲージ進捗リセット
		tutorialGageProgress_ = 0.0f;
		// 次のチュートリアルへ
		tutorialSteps_++;

		// トリガーリセット
		isNextTutorial_ = false;
	}
}

void TutorialManager::OrbReinforcementTutorial() {
	switch (tutorialStagingWayPoint_) {
	case TutorialManager::WayPoint1:
		// Bボタンが押されたら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
		    !(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {
			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialText_);
			// テクスチャ再度読み込み
			textureHandleTutorialText_ =
			    TextureManager::Load("/Image/Tutorial/10_OrbReinforcementTutorialTextUI.png");

			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialImage_);
			// テクスチャ再度読み込み
			textureHandleTutorialImage_ =
			    TextureManager::Load("/Image/Tutorial/Image_7_OrbReinforcementTutorialImageUI.png");

			// 次へ音再生
			audio_->PlayWave(audioHandles_[Audio::NextTutorial]);

			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;
		}
		break;
	case TutorialManager::WayPoint2:

		spriteNextTutorial_->SetPosition(
		    {tutorialTextPosition_.x + 450.0f, tutorialTextPosition_.y - 525.0f});

		// Bボタンが押されたら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
		    !(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {
			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialText_);
			// テクスチャ再度読み込み
			textureHandleTutorialText_ =
			    TextureManager::Load("/Image/Tutorial/OrbReinforcementTutorialTextUI.png");

			// チュートリアル画像を非表示
			displayTutorialImage_ = false;
			
			// プレイヤーを動ける状態に
			player_->SetCanAction(true);

			// 次へ音再生
			audio_->PlayWave(audioHandles_[Audio::NextTutorial]);

			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;
		}
		break;
	case TutorialManager::WayPoint3:
		// 特殊射撃強さが2以上なら
		if (player_->GetSpecialShotStrength() >= 2) {
			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialText_);
			// テクスチャ再度読み込み
			textureHandleTutorialText_ =
			    TextureManager::Load("/Image/Tutorial/5_OrbTutorialTextUI.png");
			// チュートリアルのテキスト大きさ画像に合わせて再設定
			tutorialTextSize_ = {832.0f, 192.0f};
			tutorialGagePosition_.y = 475.0f;

			// クリア音再生
			audio_->PlayWave(audioHandles_[Audio::clearTutorial]);

			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;
		}
		break;
	case TutorialManager::WayPoint4:
		// Lトリガーが押されたら
		if (joyState.Gamepad.bLeftTrigger > 25) {

			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialText_);
			// テクスチャ再度読み込み
			textureHandleTutorialText_ =
			    TextureManager::Load("/Image/Tutorial/NextTutorialUIText.png");
			// チュートリアルのテキスト大きさ画像に合わせて再設定
			tutorialTextSize_ = {832.0f, 96.0f};
			tutorialGagePosition_.y = 600.0f;

			// クリア音再生
			audio_->PlayWave(audioHandles_[Audio::clearTutorial]);

			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;
		}
		break;
	case TutorialManager::WayPoint5:
		// Bボタンが押されたら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
		    !(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {

			// 次へ音再生
			audio_->PlayWave(audioHandles_[Audio::NextTutorial]);

			// 次のチュートリアルへ
			isNextTutorial_ = true;
		}
		break;
	}

	// 敵がわいていないなら沸かせる
	if ((int)enemyManager_->GetEnemyCount() < 1 && enemySpawnInterval_ <= 0) {
		// チュートリアル用敵の出現
		enemyManager_->AddEnemy({0.0f, 25.0f, 0.0f}, Enemy::Fire, true);
		// 敵スポーン間隔リセット
		enemySpawnInterval_ = 1.0f;
	}

	// 敵スポーン間隔が0以下になる前に
	if (enemySpawnInterval_ > 0 && (int)enemyManager_->GetEnemyCount() <= 0) {
		// 敵スポーン間隔をデクリメント
		enemySpawnInterval_ -= 1.0f / 60.0f;
	}

	// 次のチュートリアルへ進む場合
	if (isNextTutorial_) {
		// テクスチャを一度アンロード
		TextureManager::Unload(textureHandleTutorialText_);
		// テクスチャ再度読み込み
		textureHandleTutorialText_ =
		    TextureManager::Load("/Image/Tutorial/11_ChangeOrbTutorialTextUI.png");
		// チュートリアルのテキスト大きさ画像に合わせて再設定
		tutorialTextSize_ = {832.0f, 192.0f};
		tutorialGagePosition_.y = 475.0f;

		// テクスチャを一度アンロード
		TextureManager::Unload(textureHandleTutorialImage_);
		// テクスチャ再度読み込み
		textureHandleTutorialImage_ =
		    TextureManager::Load("/Image/Tutorial/ChangeOrbImage1.png");
		// チュートリアル用画像表示
		displayTutorialImage_ = true;

		// プレイヤーを動けない状態に
		player_->SetCanAction(false);

		// チュートリアル個々進捗リセット
		tutorialStagingWayPoint_ = WayPoint1;

		// プレイヤーの所持オーブリセット
		player_->ResetOrbs();
		// プレイヤーにオーブを渡す
		player_->AddOrbs(PlayerBullet::Fire);
		player_->AddOrbs(PlayerBullet::Ice);
		player_->AddOrbs(PlayerBullet::Fire);
		// オーブ選択状態を氷に
		player_->SetSelectedType(PlayerBullet::Ice);
		// 選択オーブ番号を0に
		player_->SetSelectedChangeOrb(0);

		// 敵を全て削除
		enemyManager_->Delete();
		enemyManager_->SetEnemyCount(0);

		// ゲージ進捗リセット
		tutorialGageProgress_ = 0.0f;
		// 次のチュートリアルへ
		tutorialSteps_++;

		// トリガーリセット
		isNextTutorial_ = false;
	}
}

void TutorialManager::ChangeOrbTutorial() {
	switch (tutorialStagingWayPoint_) {
	case TutorialManager::WayPoint1:
		// Bボタンが押されたら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
		    !(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {
			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialText_);
			// テクスチャ再度読み込み
			textureHandleTutorialText_ =
			    TextureManager::Load("/Image/Tutorial/ChangeOrbText.png");

			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialImage_);
			// テクスチャ再度読み込み
			textureHandleTutorialImage_ =
			    TextureManager::Load("/Image/Tutorial/Image_1_OrbTutorialImageUI.png");

			// チュートリアル画像を非表示
			displayTutorialImage_ = true;

			// 次へ音再生
			audio_->PlayWave(audioHandles_[Audio::NextTutorial]);

			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;
		}
		break;
	case TutorialManager::WayPoint2:
		// Bボタンが押されたら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
		    !(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {
			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialText_);
			// テクスチャ再度読み込み
			textureHandleTutorialText_ =
			    TextureManager::Load("/Image/Tutorial/ChangeOrbTutorialText.png");

			// チュートリアルのテキスト大きさ画像に合わせて再設定
			tutorialTextSize_ = {832.0f, 96.0f};
			tutorialGagePosition_.y = 600.0f;
			// チュートリアルテキストの座標を設定
			tutorialTextPosition_ = {
			    (float)WinApp::GetInstance()->kWindowWidth / 2 + 150.0f, 600.0f};

			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialImage_);
			// テクスチャ再度読み込み
			textureHandleTutorialImage_ =
			    TextureManager::Load("/Image/Tutorial/ChangeOrbImage2.png");

			// チュートリアル画像を非表示
			displayTutorialImage_ = true;

			// 行動可能に
			player_->SetCanAction(true);
			// 移動不可能に
			player_->SetRootCanMove(false);
			// 通常射撃不可能に
			player_->SetRootCanShot(false);
			// 特殊射撃不可能に
			player_->SetRootCanSpecialShot(false);
			// オーブ選択可能に
			player_->SetRootCanSelectOrb(true);

			// 次へ音再生
			audio_->PlayWave(audioHandles_[Audio::NextTutorial]);

			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;
		}
		
		break;
	case TutorialManager::WayPoint3:

		// 次のチュートリアルへ誘導するスプライトの位置を変える
		spriteNextTutorial_->SetPosition(
		    {tutorialTextPosition_.x + 900.0f, tutorialTextPosition_.y - 175.0f});

		// ２個目のオーブが選択されたら
		if (player_->GetSelectedChangeOrb() == 1) {
			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialText_);
			// テクスチャ再度読み込み
			textureHandleTutorialText_ =
			    TextureManager::Load("/Image/Tutorial/ChangeOrbColorTutorialText.png");

			// オーブ選択不可能に
			player_->SetRootCanSelectOrb(false);
			// オーブ色変更可能に
			player_->SetRootCanChangeOrbColor(true);

			// クリア音再生
			audio_->PlayWave(audioHandles_[Audio::clearTutorial]);

			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;
		}

		break;
	case TutorialManager::WayPoint4:
		// 次のチュートリアルへ誘導するスプライトの位置を変える
		spriteNextTutorial_->SetPosition(
		    {tutorialTextPosition_.x + 900.0f, tutorialTextPosition_.y - 175.0f});
		// 変換タイプが炎の時
		if (player_->GetSelectedType() == (int)PlayerBullet::Fire) {
			// テクスチャを一度アンロード
			TextureManager::Unload(textureHandleTutorialText_);
			// テクスチャ再度読み込み
			textureHandleTutorialText_ =
			    TextureManager::Load("/Image/Tutorial/13_ChangeOrbTutorialTextUI.png");
			// チュートリアルテキスト座標
			tutorialTextPosition_ = {(float)WinApp::GetInstance()->kWindowWidth / 2, 600.0f};
			// チュートリアルのテキスト大きさ画像に合わせて再設定
			tutorialTextSize_ = {832.0f, 96.0f};
			tutorialGagePosition_.y = 600.0f;

			// クリア音再生
			audio_->PlayWave(audioHandles_[Audio::clearTutorial]);

			// オーブ色変更可能に
			player_->SetRootCanChangeOrbColor(false);
			// オーブ変換可能に
			player_->SetRootCanChangeOrb(true);

			// 次のチュートリアル段階へ
			tutorialStagingWayPoint_++;
		}
		
		break;
	case TutorialManager::WayPoint5:
		// 次のチュートリアルへ誘導するスプライトの位置を変える
		spriteNextTutorial_->SetPosition(
		    {tutorialTextPosition_.x + 900.0f, tutorialTextPosition_.y - 175.0f});

		// Xボタンが押されたら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
		    !(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_X)) {

			// クリア音再生
			audio_->PlayWave(audioHandles_[Audio::clearTutorial]);

			// 次のチュートリアルへ
			isNextTutorial_ = true;
		}
		break;
	}

	// 次のチュートリアルへ進む場合
	if (isNextTutorial_) {
		// テクスチャを一度アンロード
		TextureManager::Unload(textureHandleTutorialText_);
		// テクスチャ再度読み込み
		textureHandleTutorialText_ =
		    TextureManager::Load("/Image/Tutorial/EndTutorialText.png");

		// テクスチャを一度アンロード
		TextureManager::Unload(textureHandleTutorialImage_);
		// テクスチャ再度読み込み
		textureHandleTutorialImage_ = TextureManager::Load("/Image/Tutorial/1_MoveTutorialTextUI.png");
		// チュートリアル用画像表示
		displayTutorialImage_ = false;

		// チュートリアル個々進捗リセット
		tutorialStagingWayPoint_ = WayPoint1;

		// プレイヤーの所持オーブリセット
		player_->ResetOrbs();
		// プレイヤーにオーブを渡す
		player_->AddOrbs(PlayerBullet::Fire);
		player_->AddOrbs(PlayerBullet::Ice);
		player_->AddOrbs(PlayerBullet::Fire);

		// 移動可能に
		player_->SetRootCanMove(true);
		// 通常射撃可能に
		player_->SetRootCanShot(true);
		// 特殊射撃可能に
		player_->SetRootCanSpecialShot(true);
		// オーブ選択可能に
		player_->SetRootCanSelectOrb(true);
		// オーブ色変更可能に
		player_->SetRootCanChangeOrbColor(true);
		// オーブ変換可能に
		player_->SetRootCanChangeOrb(true);

		// 敵を全て削除
		enemyManager_->Delete();
		enemyManager_->SetEnemyCount(0);

		// ゲージ進捗リセット
		tutorialGageProgress_ = 0.0f;
		// 次のチュートリアルへ
		tutorialSteps_++;

		// トリガーリセット
		isNextTutorial_ = false;
	}
}
