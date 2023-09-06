#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <PrimitiveDrawer.h>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {
	// 重要項目初期化
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

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
	modelThunderBullet_.reset(Model::CreateFromOBJ("Wall", true)); // 雷弾
	std::vector<Model*> playerBulletModels = {
		modelBullet_.get(), // 通常弾
		modelFireBullet_.get(), // 炎弾
		modelIceBullet_.get(), // 水弾
		modelThunderBullet_.get(), // 雷弾
	}; // プレイヤー弾用モデルリストの生成


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
	player_->Initialize(playerModels, playerBulletModels); // プレイヤー

	// カメラの追従対象
	camera_->SetTarget(player_->GetWorldTransform());
	// プレイヤーにカメラのビュープロジェクション
	player_->SetViewProjection(camera_->GetViewProjection());

	//エネミーマネージャー
	enemyManager_->Initialize(std::vector<Model*>{modelEnemy_.get()});
	//enemyManager_->SetBossEnemyPlayer(player_.get());

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

	// 自弾全てについて
	for (PlayerBullet* playerBullet : player_->GetBullets()) {
		//collisionManager->ListRegister(playerBullet);
		playerBullet;
	}
	// 当たり判定
	collisionManager->CheakAllCollision();

	#ifdef _DEBUG

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
