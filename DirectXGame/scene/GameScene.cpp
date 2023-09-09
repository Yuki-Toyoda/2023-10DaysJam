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
	std::vector<Model*> playerBulletModels = { 
		modelBullet_.get(), // 通常弾
	    modelFireBullet_.get(), // 炎弾
	    modelIceBullet_.get(), // 水弾
	    modelThunderBullet_.get(), // 雷弾
	    modelDebris_.get(), // 破片エフェクト用
	}; // プレイヤー弾用モデルリストの生成

	// テクスチャ読み込み
	textureHandle1x1_ = TextureManager::Load("white1x1.png");
	textureHandleReticle_ = TextureManager::Load("/Image/Player/reticle.png");
	texturehandleOrb_ = TextureManager::Load("/Image/Player/OrbUI.png");
	texturehandleDpad_ = TextureManager::Load("/Image/Button/Dpad.png");
	texturehandleDpadArrow_N_ = TextureManager::Load("/Image/Button/DpadArrow_P.png");
	texturehandleDpadArrow_P_ = TextureManager::Load("/Image/Button/DpadArrow_N.png");
	texturehandleFireBullet_ = TextureManager::Load("/Image/Player/FireBulletUI.png");
	texturehandleIceBullet_ = TextureManager::Load("/Image/Player/IceBulletUI.png");
	texturehandleThunderBullet_ = TextureManager::Load("/Image/Player/ThunderBulletUI.png");
	textureHandleSelectedOrb_ = TextureManager::Load("/Image/Player/SelectedOrbUI.png");
	textureHandleX_ = TextureManager::Load("/Image/Player/X.png");
	textureHandleSelectArrow_L_ = TextureManager::Load("/Image/Player/selectArrowUI_L.png");
	textureHandleSelectArrow_R_ = TextureManager::Load("/Image/Player/selectArrowUI_R.png");
	std::vector<uint32_t> playerTextureHandles = {
		textureHandle1x1_, // 1x1
	    textureHandleReticle_, // 照準
	    texturehandleOrb_, // オーブ
	    texturehandleDpad_,  // 十字ボタン
		texturehandleDpadArrow_N_, 
		texturehandleDpadArrow_P_, 
		texturehandleFireBullet_,
		texturehandleIceBullet_,   
		texturehandleThunderBullet_,
	    textureHandleSelectedOrb_,
	    textureHandleX_,           
		textureHandleSelectArrow_L_,
		textureHandleSelectArrow_R_
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
		if (!enemy->GetIsInvincible()) {
			collisionManager->ListRegister(enemy);		
		}
	}
	// 敵弾全てについて
	for (EnemyBullet* enemybullet : enemyManager_->GetEnemyBullets()) {
		collisionManager->ListRegister(enemybullet);
	}
	// ボス敵全てについて
	for (BossEnemy* bossEnemy : enemyManager_->GetBossEnemis()) {
		if (!bossEnemy->GetIsInvincible()) {
			collisionManager->ListRegister(bossEnemy);
		}
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
