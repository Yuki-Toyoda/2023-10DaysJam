#include "GameScene.h"
#include "TextureManager.h"
#include "Effect/EffectManager.h"
#include <cassert>
#include <PrimitiveDrawer.h>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {
	// 重要項目初期化
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//フィールド初期化
	field_ = Field::GetInstance();
	field_->Initialize();

	// ビュープロジェクション初期化
	viewProjection_ = new ViewProjection;
	viewProjection_->farZ = 2000.0f;
	viewProjection_->Initialize();

	// モデル読み込み
	modelSkyDome_.reset(Model::CreateFromOBJ("SkyDome", true)); // 天球
	modelGround_.reset(Model::CreateFromOBJ("Ground", true)); // 地面
	std::vector<Model*> playerModels = {};  // プレイヤー用モデルリストの生成
	modelBullet_.reset(Model::CreateFromOBJ("Bullet", true)); // 通常弾
	modelFireBullet_.reset(Model::CreateFromOBJ("Bomb", true)); // 炎弾
	modelIceBullet_.reset(Model::CreateFromOBJ("Wall", true));    // 氷弾
	modelThunderBullet_.reset(Model::CreateFromOBJ("Area", true)); // 雷弾
	modelDebris_.reset(Model::CreateFromOBJ("Debris", true));      // 破片
	modelIceDebris_.reset(Model::CreateFromOBJ("IceDebris", true));   // 氷破片
	modelSpark_.reset(Model::CreateFromOBJ("Spark", true)); // 火花
	std::vector<Model*> playerBulletModels = { 
		modelBullet_.get(), // 通常弾
	    modelFireBullet_.get(), // 炎弾
	    modelIceBullet_.get(), // 水弾
	    modelThunderBullet_.get(), // 雷弾
	    modelDebris_.get(), // 破片エフェクト用
	    modelIceDebris_.get(), // 氷エフェクト用
	    modelSpark_.get() // 火花エフェクト用
	}; // プレイヤー弾用モデルリストの生成

	/// テクスチャ読み込み

	// サンプルテクスチャ
	textureHandle1x1_ = TextureManager::Load("white1x1.png");

	// 番号テクスチャ
	textureHandleNumbers_[0] = TextureManager::Load("/Image/Numbers/0.png");
	textureHandleNumbers_[1] = TextureManager::Load("/Image/Numbers/1.png");
	textureHandleNumbers_[2] = TextureManager::Load("/Image/Numbers/2.png");
	textureHandleNumbers_[3] = TextureManager::Load("/Image/Numbers/3.png");
	textureHandleNumbers_[4] = TextureManager::Load("/Image/Numbers/4.png");
	textureHandleNumbers_[5] = TextureManager::Load("/Image/Numbers/5.png");
	textureHandleNumbers_[6] = TextureManager::Load("/Image/Numbers/6.png");
	textureHandleNumbers_[7] = TextureManager::Load("/Image/Numbers/7.png");
	textureHandleNumbers_[8] = TextureManager::Load("/Image/Numbers/8.png");
	textureHandleNumbers_[9] = TextureManager::Load("/Image/Numbers/9.png");

	// ボタン用テクスチャ達
	texturehandleDpad_ = TextureManager::Load("/Image/Button/Dpad.png"); // 十字ボタン
	texturehandleDpadArrow_N_ = TextureManager::Load("/Image/Button/DpadArrow_P.png"); // 十字ボタン矢印
	texturehandleDpadArrow_P_ = TextureManager::Load("/Image/Button/DpadArrow_N.png"); // 十字ボタン矢印押下時
	texturehandleButton_X_N_ = TextureManager::Load("/Image/Button/button_x_N.png"); // Xボタン
	texturehandleButton_X_P_ = TextureManager::Load("/Image/Button/button_x_P.png"); // Xボタン押下時
	texturehandleButton_RT_N_ = TextureManager::Load("/Image/Button/rt_N.png"); // RTトリガー
	texturehandleButton_RT_P_ = TextureManager::Load("/Image/Button/rt_P.png"); // RTトリガー押下時
	texturehandleButton_RB_N_ = TextureManager::Load("/Image/Button/rb_N.png"); // RBトリガー
	texturehandleButton_RB_P_ = TextureManager::Load("/Image/Button/rb_P.png"); // RBトリガー押下時
	texturehandleButton_LT_N_ = TextureManager::Load("/Image/Button/lt_N.png"); // LTトリガー
	texturehandleButton_LT_P_ = TextureManager::Load("/Image/Button/lt_P.png"); // LTトリガー押下時

	// UIテクスチャ群
	textureHandleReticle_ = TextureManager::Load("/Image/Player/reticle.png"); // 照準
	textureHandleHeart_ = TextureManager::Load("/Image/Player/PlayerHeart.png"); // ハート
	texturehandleOrb_ = TextureManager::Load("/Image/Player/OrbUI.png"); // オーブ
	texturehandleFireBullet_ = TextureManager::Load("/Image/Player/FireBulletUI.png"); // 炎弾
	texturehandleIceBullet_ = TextureManager::Load("/Image/Player/IceBulletUI.png"); // 氷弾
	texturehandleThunderBullet_ = TextureManager::Load("/Image/Player/ThunderBulletUI.png"); // 雷弾
	texturehandleNone_ = TextureManager::Load("/Image/Player/X.png"); // 撃てない
	textureHandleSelectedOrb_ = TextureManager::Load("/Image/Player/SelectedOrbUI.png"); // 選択オーブ
	textureHandleSelectArrow_L_ =
	    TextureManager::Load("/Image/Player/selectArrowUI_L.png"); // オーブ選択左矢印
	textureHandleSelectArrow_R_ =
	    TextureManager::Load("/Image/Player/selectArrowUI_R.png"); // オーブ選択右矢印
	textureHandleRBHoldText_ =
	    TextureManager::Load("/Image/Player/RBHoldUI.png"); // RBホールドテキスト
	textureHandleChangeOrbText_ = TextureManager::Load("/Image/Player/changeOrbTextUI.png"); // 選択オーブテキスト
	textureHandleChangeOrbText2_ =
	    TextureManager::Load("/Image/Player/changeOrbTextUI2.png"); // 選択オーブテキスト
	textureHandleNeedEnemyCountText_ =
	    TextureManager::Load("/Image/Player/NeedChangeEnemyCountTextUI.png"); // オーブ変換に必要な敵数C
	textureHandleNormalShotText_ =
	    TextureManager::Load("/Image/Player/NormalShotTextUI.png"); // 通常射撃テキスト
	textureHandleSpecialShotText_ =
	    TextureManager::Load("/Image/Player/specialShotTextUI.png"); // 特殊射撃テキスト
	textureHandleMagnification[0] = TextureManager::Load("/Image/Player/x1.png"); // 倍率テクスチャx1
	textureHandleMagnification[1] = TextureManager::Load("/Image/Player/x2.png"); // 倍率テクスチャx2
	textureHandleMagnification[2] = TextureManager::Load("/Image/Player/x3.png"); // 倍率テクスチャx3

	// エフェクトテクスチャ群
	textureHandleIceWallTex[0] = TextureManager::Load("/Wall/WallTex.png");
	textureHandleIceWallTex[1] = TextureManager::Load("/Wall/WallTex2.png");
	textureHandleIceWallTex[2] = TextureManager::Load("/Wall/WallTex3.png");

	std::vector<uint32_t> playerTextureHandles = { 
		// サンプルテクスチャ
		textureHandle1x1_, // 1x1

		// 番号テクスチャ
	    textureHandleNumbers_[0], // 0
	    textureHandleNumbers_[1], // 1
	    textureHandleNumbers_[2], // 2
	    textureHandleNumbers_[3], // 3
	    textureHandleNumbers_[4], // 4
	    textureHandleNumbers_[5], // 5
	    textureHandleNumbers_[6], // 6
	    textureHandleNumbers_[7], // 7
	    textureHandleNumbers_[8], // 8
	    textureHandleNumbers_[9], // 9

		// ボタン用テクスチャ
		texturehandleDpad_, // 十字ボタン
		texturehandleDpadArrow_N_, // 十字ボタン矢印
		texturehandleDpadArrow_P_, // 十字ボタン矢印押下時
		texturehandleButton_X_N_, // Xボタン
		texturehandleButton_X_P_, // Xボタン押下時
		texturehandleButton_RT_N_, // RTトリガー
		texturehandleButton_RT_P_, // RTトリガー押下時
		texturehandleButton_RB_N_, // RBトリガー
		texturehandleButton_RB_P_, // RBトリガー押下時
		texturehandleButton_LT_N_, // LTトリガー
		texturehandleButton_LT_P_, // LTトリガー押下時

		// UIテクスチャ
	    textureHandleReticle_, // 照準
	    textureHandleHeart_, // ハート
	    texturehandleOrb_, // オーブ 
		texturehandleFireBullet_, // 炎弾
		texturehandleIceBullet_, // 氷弾
		texturehandleThunderBullet_, // 雷弾
	    texturehandleNone_, // 撃てない
	    textureHandleSelectedOrb_, // オーブ選択
		textureHandleSelectArrow_L_, // オーブ選択左矢印
		textureHandleSelectArrow_R_, // オーブ選択右矢印
	    textureHandleRBHoldText_, // RBホールドテキスト
	    textureHandleChangeOrbText_, // 選択オーブテキスト
	    textureHandleChangeOrbText2_, // 選択オーブテキスト2
	    textureHandleNeedEnemyCountText_, // 変換に必要な敵数テキスト
	    textureHandleNormalShotText_, // 通常射撃テキスト
	    textureHandleSpecialShotText_, // 特殊射撃テキスト
	    textureHandleMagnification[0], // 1x
	    textureHandleMagnification[1], // 2x
	    textureHandleMagnification[2], // 3x

		// エフェクト用テクスチャ
		textureHandleIceWallTex[0], // 氷壁1段階目
	    textureHandleIceWallTex[1], // 氷壁2段階目
	    textureHandleIceWallTex[2], // 氷壁3段階目
	};

	// 効果音
	soundHandleFootStep_[0] = audio_->LoadWave("/Audio/SE/FootStep1.wav"); // 歩行音1
	soundHandleFootStep_[1] = audio_->LoadWave("/Audio/SE/FootStep2.wav"); // 歩行音2
	soundHandleJump_ = audio_->LoadWave("/Audio/SE/Jump.wav"); // ジャンプ音
	soundHandleLanding_ = audio_->LoadWave("/Audio/SE/Landing.wav"); // 着地音
	soundHandlePlayerDamage_ = audio_->LoadWave("/Audio/SE/PlayerDamage.wav"); // ダメージ音
	soundHandleShot_ = audio_->LoadWave("/Audio/SE/Shot.wav"); // 通常射撃
	soundHandleAddOrb_ = audio_->LoadWave("/Audio/SE/AddOrb.wav"); // オーブ追加音
	soundHandleSpecialShot_ = audio_->LoadWave("/Audio/SE/SpecialShot.wav"); // 特殊射撃
	soundHandleFireShot_ = audio_->LoadWave("/Audio/SE/FireBullet.wav");    // 炎弾着弾音
	soundHandleDeployIceWall_ = audio_->LoadWave("/Audio/SE/DeployIceWall.wav"); // 氷弾展開音
	soundHandleDamageIceWall_ = audio_->LoadWave("/Audio/SE/DamageIceWall.wav"); // 氷弾ダメージ音
	soundHandleDestroyIceWall_ = audio_->LoadWave("/Audio/SE/DestroyIceWall.wav"); // 氷弾破壊音
	soundHandleDeployStartThunderArea_ =
	    audio_->LoadWave("/Audio/SE/DeployStartThunderArea.wav"); // 雷エリア展開開始音
	soundHandleDeployThunderArea_ = audio_->LoadWave("/Audio/SE/DeployThunderArea.wav"); // 雷エリア展開音
	soundHandleDeployEndThunderArea_ =
	    audio_->LoadWave("/Audio/SE/DeployEndThunderArea.wav"); // 雷エリア展開終了音
	soundHandleDeployOrbChoice_ =
	    audio_->LoadWave("/Audio/SE/OrbChoice.wav"); // オーブ選択音
	soundHandleDeployChangeOrbSuccess_ =
	    audio_->LoadWave("/Audio/SE/OrbChangeSuccesful.wav"); // オーブ変換成功音
	soundHandleDeployChangeOrbFail_ =
	    audio_->LoadWave("/Audio/SE/OrbChangeError.wav"); // オーブ変換失敗音
	soundHandleDeployChoiceFireBullet_ =
	    audio_->LoadWave("/Audio/SE/ChoiceFireBullet.wav"); // 炎弾選択音
	soundHandleDeployChoiceIceBullet_ =
	    audio_->LoadWave("/Audio/SE/ChoiceIceBullet.wav"); // 氷弾選択音

	std::vector<uint32_t> playerAudioHandles = {
		soundHandleFootStep_[0], // 歩行音1
	    soundHandleFootStep_[1], // 歩行音2
	    soundHandleJump_, // ジャンプ
	    soundHandleLanding_, // 着地
	    soundHandlePlayerDamage_, // プレイヤーのダメージ音
	    soundHandleShot_, // 通常射撃音
	    soundHandleAddOrb_, // オーブ追加音
		soundHandleSpecialShot_, // 特殊射撃音
	    soundHandleFireShot_, // 炎弾着弾音
	    soundHandleDeployIceWall_, // 氷弾展開音
	    soundHandleDamageIceWall_, // 氷弾ダメージ音
	    soundHandleDestroyIceWall_, // 氷壁破壊音
	    soundHandleDeployStartThunderArea_, // 雷エリア展開開始音
	    soundHandleDeployThunderArea_, // 雷エリア展開音
	    soundHandleDeployEndThunderArea_, // 雷エリア展開終了音
		soundHandleDeployOrbChoice_, // オーブ選択音
		soundHandleDeployChangeOrbSuccess_, // オーブ変換成功音
		soundHandleDeployChangeOrbFail_, // オーブ変換失敗音
	    soundHandleDeployChoiceFireBullet_, // 炎弾選択音
	    soundHandleDeployChoiceIceBullet_, // 氷弾選択音
	};

	// エフェクトマネージャーの取得
	effectManager_ = EffectManager::GetInstance();
	// 取得したエフェクトマネージャーの初期化
	effectManager_->Initialize();

	modelEnemy_.reset(Model::CreateFromOBJ("Fish", true));//エネミー
	modelBossEnemy_.reset(Model::CreateFromOBJ("BossFish", true)); // ボスエネミー
	modelEnemyBullet_.reset(Model::CreateFromOBJ("EnemyBullet", true)); // エネミーバレット

	// デバックカメラ無効
	enableDebugCamera_ = false;

	// クラスインスタンス生成
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720); // デバックカメラ
	debugCamera_->SetFarZ(2000.0f);
	camera_ = std::make_unique<Camera>(); // カメラ
	skyDome_ = std::make_unique<SkyDome>(); // 天球
	ground_ = std::make_unique<Ground>(); // 地面
	player_ = std::make_unique<Player>(); // プレイヤー

	enemyManager_ = EnemyManager::GetInstance();// エネミーマネージャー
	enemyManager_->SetPlayer(player_.get());

	// 生成インスタンス初期化
	camera_->Intialize(); // カメラ
	skyDome_->Intialize(modelSkyDome_.get()); // 天球
	ground_->Intialize(modelGround_.get()); // 地面
	player_->Initialize(
	    playerModels, playerBulletModels, playerTextureHandles, playerAudioHandles); // プレイヤー

	// カメラの追従対象
	camera_->SetTarget(player_->GetWorldTransform());
	// プレイヤーにカメラのビュープロジェクション
	player_->SetViewProjection(camera_->GetViewProjection());
	// エフェクトマネージャーにもカメラのビュープロジェクションをセット
	effectManager_->SetViewProjection(camera_->GetViewProjection());
	// カメラにカメラシェイク強さの変数ポインタを渡す
	camera_->SetShakeStrength(player_->GetShakeStrength());

	//テクスチャハンドル
	std::vector<uint32_t> enemyTextureHandles = {
		TextureManager::GetInstance()->Load("./Resources/white1x1.png"),
		TextureManager::GetInstance()->Load("./Resources/Enemy/EnemyFire.png"),
		TextureManager::GetInstance()->Load("./Resources/Enemy/EnemyIce.png"),
		TextureManager::GetInstance()->Load("./Resources/Enemy/EnemyThunder.png"),
	};

	//エネミーマネージャー
	enemyManager_->Initialize(
	    std::vector<Model*>{modelEnemy_.get()}, enemyTextureHandles,
	    std::vector<Model*>{modelBossEnemy_.get()}, std::vector<Model*>{modelEnemyBullet_.get()});

	// 衝突マネージャー
	collisionManager.reset(new CollisionManager);

	// 現在のシーン
	currentScene_ = Main;
	// 次のシーン
	nextScene_ = Main;

	// フェードインしているか
	isFadeIn_ = false;
	// フェードアウトしているか
	isFadeOut_ = false;
	// フェードタイマー
	fadeTimer_ = 0;

	// フェードテクスチャ
	fadeTextureHandle_ = TextureManager::Load("white1x1.png");
	// フェードポジション
	fadePosition_ = {float(WinApp::kWindowWidth) / 2.0f, float(WinApp::kWindowHeight) / 2.0f};
	// フェード色
	fadeColor_ = {0.0f, 0.0f, 0.0f, 1.0f};
	// フェードアンカーポイント
	Vector2 anchorPoint = {0.5f, 0.5f};
	// フェードスプライト
	fadeSprite_.reset(Sprite::Create(fadeTextureHandle_, fadePosition_, fadeColor_, anchorPoint));
	// フェードサイズ
	fadeSize_ = {float(WinApp::kWindowWidth), float(WinApp::kWindowHeight)};
	fadeSprite_->SetSize(fadeSize_);

	// ゲームパッドの状態取得
	input_->GetJoystickState(0, joyState);
	preJoyState = joyState;

}

void GameScene::Update() {

	// ゲームパッドの状態取得
	preJoyState = joyState;
	input_->GetJoystickState(0, joyState);
	

	switch (currentScene_) {
	case GameScene::Title:
		TitleUpdate();
		break;
	case GameScene::Tutorial:
		TutorialUpdate();
		break;
	case GameScene::Main:
		if (!theGameIsOver) {
			MainUpdate();
		}
		break;
	case GameScene::GameClear:
		GameClearUpdate();
		break;
	case GameScene::GameOver:
		GameOverUpdate();
		break;
	default:
		break;
	}

	//フェードインアウト
	FadeInOutUpdate();
	

}

void GameScene::TitleUpdate() {}

void GameScene::TutorialUpdate() {}

void GameScene::MainUpdate() {

	// リスト削除
	enemyManager_->DeleteEnemy();
	enemyManager_->DeleteEnemyBullet();

	// 更新処理全般
	camera_->Update();        // カメラ
	skyDome_->Update();       // 天球
	ground_->Update();        // 地面
	player_->Update();        // プレイヤー
	effectManager_->Update(); // エフェクトマネージャー
	enemyManager_->Update();  // エネミーマネージャー

	// デバックカメラ有効時
	if (enableDebugCamera_) {
		debugCamera_->Update(); // デバックカメラ

		// ビュープロジェクションをデバックカメラのものに設定する
		viewProjection_->matView = debugCamera_->GetViewProjection().matView;
		viewProjection_->matProjection = debugCamera_->GetViewProjection().matProjection;

	} else {
		// ビュープロジェクションを追従カメラのものに設定する
		viewProjection_ = camera_->GetViewProjection();
	}

	// 行列を定数バッファに転送
	viewProjection_->TransferMatrix();

	// リストをクリア
	collisionManager->ListClear();
	// コライダーをリストに登録
	// 自機について
	collisionManager->ListRegister(player_.get());
	// 敵全てについて
	for (Enemy* enemy : enemyManager_->GetEnemies()) {
		collisionManager->ListRegister(enemy);
	}
	// 敵弾全てについて
	for (EnemyBullet* enemybullet : enemyManager_->GetEnemyBullets()) {
		collisionManager->ListRegister(enemybullet);
	}
	// ボス敵全てについて
	for (BossEnemy* bossEnemy : enemyManager_->GetBossEnemis()) {
		collisionManager->ListRegister(bossEnemy);
	}
	// 自弾全てについて
	for (PlayerBullet* playerBullet : player_->GetBullets()) {
		collisionManager->ListRegister(playerBullet);
	}
	// 当たり判定
	collisionManager->CheakAllCollision();

	//ゲームオーバーか?
	if (player_->GetHp() <= 0) {
		FadeInOutSetUp(GameOver);
		theGameIsOver = true;
	}

	#ifdef _DEBUG

	// フィールドの更新
	field_->Update();

	ImGui::Begin("Debug");
	ImGui::Checkbox("activeDebugCamera", &enableDebugCamera_);

	ImGui::End();

#endif // _DEBUG

}

void GameScene::GameClearUpdate() {}

void GameScene::GameOverUpdate() {

	//Aボタンでタイトルへ
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
	    !(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
		FadeInOutSetUp(Main);
	}

}

void GameScene::FadeInOutUpdate() {

	if (isFadeOut_) {
		if (--fadeTimer_ == 0) {
			// 次のシーンへ
			isFadeOut_ = false;
			isFadeIn_ = true;
			fadeTimer_ = kFadeTime_;
			currentScene_ = nextScene_;
			fadeColor_.w = 1.0f;

			//セットアップ
			switch (nextScene_) {
			case GameScene::Title:
				TitleSetup();
				break;
			case GameScene::Tutorial:
				TutorialSetup();
				break;
			case GameScene::Main:
				MainSetup();
				break;
			case GameScene::GameClear:
				GameClearSetup();
				break;
			case GameScene::GameOver:
				GameOverSetup();
				break;
			default:
				break;
			}

		} else {
			fadeColor_.w = float(kFadeTime_ - fadeTimer_) / float(kFadeTime_);
		}
		fadeSprite_->SetColor(fadeColor_);
	} else if (isFadeIn_) {
		if (--fadeTimer_ == 0) {
			isFadeIn_ = false;
			fadeColor_.w = 0.0f;
		} else {
			fadeColor_.w = float(fadeTimer_) / float(kFadeTime_);
		}
		fadeSprite_->SetColor(fadeColor_);
	}

}

void GameScene::TitleSetup() {}

void GameScene::TutorialSetup() {}

void GameScene::MainSetup() {

	theGameIsOver = false;

	// カメラ
	camera_->SetUp();
	// プレイヤー
	player_->Setup();
	// エフェクトマネージャー
	effectManager_->Initialize();
	// エネミーマネージャー
	enemyManager_->Reset();

}

void GameScene::GameClearSetup() {}

void GameScene::GameOverSetup() {}

void GameScene::FadeInOutSetUp(SceneName nextScene) {
	
	if (!isFadeOut_ && !isFadeIn_) {
		isFadeOut_ = true;
		nextScene_ = nextScene;
		fadeTimer_ = kFadeTime_;
	}

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	//ライン描画のビュープロジェクション設定
	PrimitiveDrawer::GetInstance()->SetViewProjection(viewProjection_);

	switch (currentScene_) {
	case GameScene::Title:
		TitleDraw(commandList);
		break;
	case GameScene::Tutorial:
		TutorialDraw(commandList);
		break;
	case GameScene::Main:
		MainDraw(commandList);
		break;
	case GameScene::GameClear:
		GameClearDraw(commandList);
		break;
	case GameScene::GameOver:
		GameOverDraw(commandList);
		break;
	default:
		break;
	}

}

void GameScene::TitleDraw(ID3D12GraphicsCommandList* commandList) { 

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//フェードインアウト
	FadeInOutDraw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameScene::TutorialDraw(ID3D12GraphicsCommandList* commandList) {

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	
	// フェードインアウト
	FadeInOutDraw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameScene::MainDraw(ID3D12GraphicsCommandList* commandList) {

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	skyDome_->Draw(*viewProjection_);       // 天球
	ground_->Draw(*viewProjection_);        // 地面
	player_->Draw(*viewProjection_);        // プレイヤー
	effectManager_->Draw(*viewProjection_); // エフェクトマネージャー
	enemyManager_->Draw(*viewProjection_);  // エネミー

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	player_->SpriteDraw(); // プレイヤー

	// フェードインアウト
	FadeInOutDraw();

#ifdef _DEBUG
	player_->ColliderDraw(enableDebugCamera_);
	enemyManager_->ColliderDraw();
#endif // _DEBUG

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameScene::GameClearDraw(ID3D12GraphicsCommandList* commandList) {

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// フェードインアウト
	FadeInOutDraw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameScene::GameOverDraw(ID3D12GraphicsCommandList* commandList) {
	
#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// フェードインアウト
	FadeInOutDraw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameScene::FadeInOutDraw() {

	if (isFadeIn_ || isFadeOut_) {
		fadeSprite_->Draw();
	}

}
