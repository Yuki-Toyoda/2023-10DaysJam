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

	// エフェクトマネージャーの取得
	effectManager_ = EffectManager::GetInstance();
	// 取得したエフェクトマネージャーの初期化
	effectManager_->Initialize();

	modelEnemy_.reset(Model::CreateFromOBJ("Fish", true));//エネミー

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
	player_->Initialize(playerModels, playerBulletModels, playerTextureHandles); // プレイヤー

	// カメラの追従対象
	camera_->SetTarget(player_->GetWorldTransform());
	// プレイヤーにカメラのビュープロジェクション
	player_->SetViewProjection(camera_->GetViewProjection());
	// エフェクトマネージャーにもカメラのビュープロジェクションをセット
	effectManager_->SetViewProjection(camera_->GetViewProjection());
	// カメラにカメラシェイク強さの変数ポインタを渡す
	camera_->SetShakeStrength(player_->GetShakeStrength());

	//テクスチャハンドル
	std::vector<uint32_t> textureHandles = {
		TextureManager::GetInstance()->Load("./Resources/white1x1.png"),
		TextureManager::GetInstance()->Load("./Resources/Enemy/EnemyFire.png"),
		TextureManager::GetInstance()->Load("./Resources/Enemy/EnemyIce.png"),
		TextureManager::GetInstance()->Load("./Resources/Enemy/EnemyThunder.png"),
	};

	//エネミーマネージャー
	enemyManager_->Initialize(std::vector<Model*>{modelEnemy_.get()}, textureHandles);

	// 衝突マネージャー
	collisionManager.reset(new CollisionManager);

}

void GameScene::Update() {
	
	//リスト削除
	enemyManager_->DeleteEnemy();
	enemyManager_->DeleteEnemyBullet();

	// 更新処理全般
	camera_->Update(); // カメラ
	skyDome_->Update(); // 天球
	ground_->Update(); // 地面
	player_->Update(); // プレイヤー
	effectManager_->Update(); // エフェクトマネージャー
	enemyManager_->Update();// エネミー

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

	#ifdef _DEBUG

	//フィールドの更新
	field_->Update();

	ImGui::Begin("Debug");
	ImGui::Checkbox("activeDebugCamera", &enableDebugCamera_);
	
	ImGui::End();

#endif // _DEBUG

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	//ライン描画のビュープロジェクション設定
	PrimitiveDrawer::GetInstance()->SetViewProjection(viewProjection_);

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

	skyDome_->Draw(*viewProjection_); // 天球
	ground_->Draw(*viewProjection_); // 地面
	player_->Draw(*viewProjection_); // プレイヤー
	effectManager_->Draw(*viewProjection_); // エフェクトマネージャー
	enemyManager_->Draw(*viewProjection_);// エネミー

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

	#ifdef _DEBUG
	player_->ColliderDraw(enableDebugCamera_);
	enemyManager_->ColliderDraw();
#endif // _DEBUG

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
