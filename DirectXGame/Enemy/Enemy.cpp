#include "Enemy.h"
#include "EnemyBullet.h"
#include "EnemyManager.h"
#include <cassert>
#include "MyMath.h"
#include "Charactor/CharactorList.h"
#include "../config/GlobalVariables.h"
#include "Collision/ColliderShape/OBB.h"
#include "Ambient/Field.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="models">モデルデータ配列</param>
void Enemy::Initialize(const std::vector<Model*>& models, uint32_t textureHandle, EnemyType enemyType,
    Vector3 posioton, uint32_t hp, EnemyManager* enemyManager, Player* player,
    std::list<BossEnemy*>* bossEnemies) {
	
	// NULLポインタチェック
	assert(models.front());

	// ベースキャラクター初期化
	BaseCharacter::Initialize(models);
	textureHandle_ = textureHandle;

	// 初期座標
	worldTransform_.translation_ = posioton;
	worldTransform_.UpdateMatrix();

	// デスフラグ
	isDead_ = false;

	//タイプ
	enemyType_ = enemyType;

	//状態
	enemyState_ = Wait;

	// ボスとの距離
	distanceToBoss_ = 0.0f;
	// ボスとの回転角
	rotationToBoss_ = {0.0f,0.0f,0.0f};
	targetWorldTransform.Initialize();

	// 突進準備用
	// スタート位置
	preRushStartPos_ = {0.0f,0.0f,0.0f};
	// ゴール位置
	preRushEndPos_ = {0.0f, 0.0f, 0.0f};
	// T
	preRushT_ = 0.0f;

	// 体力
	hp_ = hp;

	// 無敵か
	isInvincible_ = false;

	// 無敵タイマー
	invincibilityTimer_ = 0;

	// 衝突無敵タイマー
	collisionInvincibilityTime_ = 20;

	// 衝突属性を設定
	SetCollisionAttribute(0xfffffffd);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(0x00000002);
	// タグ
	tag_ = TagEnemy;

	// エネミーマネージャー
	enemyManager_ = enemyManager;
	// プレイヤー
	player_ = player;
	// ボス
	bossEnemies_ = bossEnemies;

	// コライダーの形
	OBB* obb = new OBB();
	obb->Initialize(GetWorldPosition(), worldTransform_.rotation_, worldTransform_.scale_);
	colliderShape_ = obb;

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "Enemy";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加
	globalVariables->AddItem(groupName, "MoveSpeed", moveSpeed_);
	globalVariables->AddItem(groupName, "MoveRotateSpeed", moveRotateSpeed_);
	globalVariables->AddItem(groupName, "MoveToPlayerSpeed", moveToPlayerSpeed_);
	globalVariables->AddItem(groupName, "BulletSpeed", bulletSpeed_);
	globalVariables->AddItem(groupName, "RushSpeed", rushSpeed_);
	globalVariables->AddItem(groupName, "CollisionInvincibilityTimer", int(collisionInvincibilityTime_));

	colliderShape_->AddToGlobalVariables(groupName);

}

/// <summary>
/// 更新
/// </summary>
void Enemy::Update() {

	switch (enemyState_) {
	case Enemy::Wait:
		Waiting();
		break;
	case Enemy::Follow:
		Following();
		break;
	case Enemy::PreRush:
		PreRushing();
		break;
	case Enemy::Rush:
		Rushing();
	default:
		break;
	}

	//無敵タイマー処理
	if (isInvincible_) {
		if (--invincibilityTimer_ == 0) {
			isInvincible_ = false;
		}
	}

	//グローバル変数適用
	ApplyGlobalVariables();

	//トランスフォームの制限確認
	Field* field = Field::GetInstance();
	if (worldTransform_.translation_.x <= field->GetMin().x ||
	    worldTransform_.translation_.y <= field->GetMin().y ||
	    worldTransform_.translation_.z <= field->GetMin().z ||
	    worldTransform_.translation_.x >= field->GetMax().x ||
	    worldTransform_.translation_.y >= field->GetMax().y ||
	    worldTransform_.translation_.z >= field->GetMax().z ) {
		//死ぬ
		Dead();
	}

	// 基底クラス更新
	BaseCharacter::Update();

	//コライダー更新
	colliderShape_->Update(GetWorldPosition(), worldTransform_.rotation_, colliderShape_->GetSize());

}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
void Enemy::Draw(const ViewProjection& viewProjection) {

	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection, textureHandle_);
	}

}

// 衝突時に呼ばれる関数
void Enemy::OnCollision(Collider* collision) {

	switch (collision->GetTag()) {

	case TagPlayer:
		CollisionPlayer();
		break;
	case TagPlayerBulletNone:
		CollisionBulletNone();
		break;
	case TagPlayerBulletFire:
		CollisionBulletFire();
		break;
	case TagPlayerBulletIce:
		CollisionBulletIce();
		break;
	case TagPlayerBulletThunder:
		CollisionBulletThunder();
		break;
	case TagBossEnemy:
		CollisionBossEnemy(collision->GetBossEnemy());
		break;
	default:
		break;
	}

}

void Enemy::Move() {

	//回転
	// 回転速度
	Vector3 velocity(0.0f, moveRotateSpeed_, 0.0f);
	worldTransform_.rotation_ = worldTransform_.rotation_ + velocity;
	
	//移動
	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = {0.0f, 0.0f, moveSpeed_};

	velocity = MyMath::Transform(velocity, MyMath::MakeRotateXYZMatrix(worldTransform_.rotation_));
	worldTransform_.translation_ = worldTransform_.translation_ + velocity;

}

void Enemy::MoveToPlayer() {

	// 自キャラのワールド座標を取得する
	Vector3 playerPos = player_->GetWorldPosition();
	// 敵のワールド座標を取得する
	Vector3 enemyrPos = GetWorldPosition();
	// 敵->自キャラの差分ベクトルを求める
	Vector3 toPlayer = playerPos - enemyrPos;
	// ベクトルの正規化
	toPlayer = MyMath::Normalize(toPlayer);
	//速度
	Vector3 velocity = toPlayer * moveToPlayerSpeed_;
	//回転
	MoveRotation(toPlayer);

	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ = worldTransform_.translation_ + velocity;

}

void Enemy::Attack() {

	// 弾の速度
	Vector3 velocity(0, 0, bulletSpeed_);
	// 弾の位置
	Vector3 position = GetWorldPosition();

	position.z += std::cosf(worldTransform_.rotation_.y) * std::cosf(worldTransform_.rotation_.x) * 5.0f; // コサイン
	position.x += std::sinf(worldTransform_.rotation_.y) * std::cosf(worldTransform_.rotation_.x) * 5.0f;
	position.y += -std::sinf(worldTransform_.rotation_.x) * std::cosf(worldTransform_.rotation_.z) * 5.0f;

	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = MyMath::Transform(velocity, MyMath::MakeRotateXYZMatrix(worldTransform_.rotation_));
	enemyManager_->AddEnemyBullet(position, velocity);

}

void Enemy::PreRushStart() {

	enemyState_ = PreRush;
	//位置設定
	preRushStartPos_ = worldTransform_.translation_;
	preRushEndPos_ = worldTransform_.translation_;
	// 自キャラのワールド座標を取得する
	Vector3 playerPos = player_->GetWorldPosition();
	// 敵弾のワールド座標を取得する
	Vector3 enemyrPos = GetWorldPosition();
	// 敵弾->自キャラの差分ベクトルを求める
	Vector3 toPlayer = playerPos - enemyrPos;
	// ベクトルの正規化
	toPlayer = MyMath::Normalize(toPlayer);
	// 速度
	preRushEndPos_ = preRushEndPos_ - (toPlayer * preRushDistance_);

	preRushT_ = 0.0f;

}

void Enemy::PreRushing() {


	//イージングで後ろに下がる
	if (preRushT_ < 1.0f) {
		
		preRushT_ += preRushTSpeed_;
		if (preRushT_ >= 1.0f) {
			preRushT_ = 1.0f;
		}

		worldTransform_.translation_ = {
			MyMath::Linear(preRushT_, preRushStartPos_.x, preRushEndPos_.x),
			MyMath::Linear(preRushT_, preRushStartPos_.y, preRushEndPos_.y),
			MyMath::Linear(preRushT_, preRushStartPos_.z, preRushEndPos_.z)
		};
	} else {
		// イージング終了で突進開始
		RushStart();
	}

}

void Enemy::RushStart() {

	// 自キャラのワールド座標を取得する
	Vector3 playerPos = player_->GetWorldPosition();
	// 敵弾のワールド座標を取得する
	Vector3 enemyrPos = GetWorldPosition();
	// 敵弾->自キャラの差分ベクトルを求める
	Vector3 toPlayer = playerPos - enemyrPos;
	// ベクトルの正規化
	toPlayer = MyMath::Normalize(toPlayer);
	// 速度
	velocity_ = toPlayer * rushSpeed_;

	//回転
	//  Y軸周りの角度(Θy)
	worldTransform_.rotation_.y = std::atan2f(toPlayer.x, toPlayer.z);
	// 横軸方向の長さを求める
	float length = MyMath::Length(Vector3{toPlayer.x, 0.0f, toPlayer.z});
	// X軸周りの角度(Θx)
	worldTransform_.rotation_.x = std::atan2f(-toPlayer.y, length);

	enemyState_ = Rush;

}

void Enemy::Rushing() {

	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;

}

void Enemy::Waiting() {

	//  自キャラのワールド座標を取得する
	Vector3 playerPos = player_->GetWorldPosition();
	// 敵のワールド座標を取得する
	Vector3 enemyrPos = GetWorldPosition();
	// 敵->自キャラの差分ベクトルを求める
	Vector3 toPlayer = playerPos - enemyrPos;
	//距離を求める
	float length = MyMath::Length(toPlayer);
	//近いならプレイヤーに近づく
	if (closeToThePlayer_ >= length) {
		MoveToPlayer();
	} else {
		Move();
	}


}

void Enemy::Following() {

	// ワールド座標を取得する
	Vector3 pos = GetWorldPosition();
	// 目指すワールド座標を取得する
	targetWorldTransform.UpdateMatrix();
	Vector3 bossPos = Vector3(
	    targetWorldTransform.matWorld_.m[3][0], targetWorldTransform.matWorld_.m[3][1],
	    targetWorldTransform.matWorld_.m[3][2]);

	float t = 0.025f;
	worldTransform_.translation_ = {
	    MyMath::Linear(t, pos.x, bossPos.x), MyMath::Linear(t, pos.y, bossPos.y),
	    MyMath::Linear(t, pos.z, bossPos.z)
	};

	// 回転
	MoveRotation(bossPos - pos);

	if (JoiningBossEnemy_->GetBossEnemyState() == BossEnemy::PreAttackCommand) {
		// 回転
		worldTransform_.rotation_ = JoiningBossEnemy_->GetWorldTransform().rotation_;
	}
	if (JoiningBossEnemy_->GetBossEnemyState() == BossEnemy::AttackCommand) {
		PreRushStart();
	}

}

void Enemy::Join(BossEnemy* bossEnemy) {

	enemyState_ = Follow;

	// 衝突属性を設定
	SetCollisionAttribute(0xfffffff9);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(0x00000006);

	JoiningBossEnemy_ = bossEnemy;
	
	targetWorldTransform.parent_ = &JoiningBossEnemy_->GetWorldTransform();
	/*
	distanceToBoss_ = 30.0f;

	rotationToBoss_.z = float(std::numbers::pi) * 2.0f *
	                float(JoiningBossEnemy_->GetEnemiesJoiningNum()) /
	                float(JoiningBossEnemy_->GetEnemiesJoiningNumMax());

	targetWorldTransform.translation_.x = distanceToBoss_ * cosf(rotationToBoss_.z);
	targetWorldTransform.translation_.y = distanceToBoss_ * sinf(rotationToBoss_.z);
	*/

	Vector3* transform = JoiningBossEnemy_->GetUnitTtransform();
	targetWorldTransform.translation_ = transform[JoiningBossEnemy_->GetEnemiesJoiningNum()];

}

void Enemy::MoveRotation(Vector3 toPosition) {

	Vector3 rotate = worldTransform_.rotation_;
	Vector3 target = worldTransform_.rotation_;
	float pi = float(std::numbers::pi);

	//  Y軸周りの角度(Θy)
	target.y = std::atan2f(toPosition.x, toPosition.z);
	// 横軸方向の長さを求める
	float length = MyMath::Length(Vector3{toPosition.x, 0.0f, toPosition.z});
	// X軸周りの角度(Θx)
	target.x = std::atan2f(-toPosition.y, length);

	// ターゲット
	while (target.x > pi) {
		target.x -= pi;
	}
	while (target.x < -pi) {
		target.x += pi;
	}

	while (target.y > pi) {
		target.y -= pi;
	}
	while (target.y < -pi) {
		target.y += pi;
	}

	while (target.z > pi) {
		target.z -= pi;
	}
	while (target.z < -pi) {
		target.z += pi;
	}

	// rotate
	while (rotate.x > pi) {
		rotate.x -= pi;
	}
	while (rotate.x < -pi) {
		rotate.x += pi;
	}

	while (rotate.y > pi) {
		rotate.y -= pi;
	}
	while (rotate.y < -pi) {
		rotate.y += pi;
	}

	while (rotate.z > pi) {
		rotate.z -= pi;
	}
	while (rotate.z < -pi) {
		rotate.z += pi;
	}

	// 回転割合
	float t = 0.05f;
	worldTransform_.rotation_ = {
	    MyMath::Linear(t, rotate.x, target.x), MyMath::Linear(t, rotate.y, target.y), rotate.z};


}

void Enemy::Dead() {

	//死亡フラグをたてる
	isDead_ = true;
	enemyManager_->SetEnemyCount(enemyManager_->GetEnemyCount() - 1);

}

void Enemy::HpFluctuation(int32_t damage, uint32_t InvincibilityTime) {

	if (isInvincible_) {
		return;
	}

	hp_ -= damage;
	if (hp_ <= 0) {
		Dead();
	} else {
		isInvincible_ = true;
		invincibilityTimer_ = InvincibilityTime;
	}

}

void Enemy::CollisionBulletNone() {

	HpFluctuation(
	    player_->GetBulletDamage(PlayerBullet::BulletType::None), collisionInvincibilityTime_);

	if (isDead_) {
		// オーブ取得
		if (enemyType_ == EnemyType::Fire) {
			player_->AddOrbs(PlayerBullet::BulletType::Fire);
		} else if (enemyType_ == EnemyType::Ice) {
			player_->AddOrbs(PlayerBullet::BulletType::Ice);
		} else if (enemyType_ == EnemyType::Thunder) {
			player_->AddOrbs(PlayerBullet::BulletType::Thunder);
		}
	}

}

void Enemy::CollisionBulletFire() {

	HpFluctuation(
	    player_->GetBulletDamage(PlayerBullet::BulletType::Fire), collisionInvincibilityTime_);

}

void Enemy::CollisionBulletIce() {

	HpFluctuation(
	    player_->GetBulletDamage(PlayerBullet::BulletType::Ice), collisionInvincibilityTime_);

}

void Enemy::CollisionBulletThunder() {

	HpFluctuation(
	    player_->GetBulletDamage(PlayerBullet::BulletType::Thunder), collisionInvincibilityTime_);

}

void Enemy::CollisionBossEnemy(BossEnemy* bossEnemy) {

	if (enemyState_ == Wait && bossEnemy->GetBossEnemyState() == BossEnemy::Collect) {
		Join(bossEnemy);
	}

}

void Enemy::CollisionPlayer() {}

void Enemy::ApplyGlobalVariables() {

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "Enemy";

	// メンバ変数の調整項目をグローバル変数に追加
	moveSpeed_ = globalVariables->GetFloatValue(groupName, "MoveSpeed");
	moveRotateSpeed_ = globalVariables->GetFloatValue(groupName, "MoveRotateSpeed");
	moveToPlayerSpeed_ = globalVariables->GetFloatValue(groupName, "MoveToPlayerSpeed");
	bulletSpeed_ = globalVariables->GetFloatValue(groupName, "BulletSpeed");
	rushSpeed_ = globalVariables->GetFloatValue(groupName, "RushSpeed");
	collisionInvincibilityTime_ =
	    uint32_t(globalVariables->GetIntValue(groupName, "CollisionInvincibilityTimer"));

	colliderShape_->ApplyGlobalVariables(groupName);

}
