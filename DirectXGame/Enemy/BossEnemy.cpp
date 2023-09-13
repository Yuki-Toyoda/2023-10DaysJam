#include "BossEnemy.h"
#include "MyMath.h"
#include "Charactor/Player/Player.h"
#include "../config/GlobalVariables.h"
#include <cassert>
#include <stdio.h>
#include <string>
#include "Enemy.h"
#include "Collision/ColliderShape/OBB.h"
#include "EnemyManager.h"
#include <Ambient/Field.h>

/// <summary>
/// 初期化
/// </summary>
/// <param name="models">モデルデータ配列</param>
void BossEnemy::Initialize(
    const std::vector<Model*>& models, uint32_t textureHandle, EnemyManager* enemyManager,
    Player* player, uint32_t hp, const std::vector<uint32_t>& audioHandles) {

	// NULLポインタチェック
	assert(models.front());

	// ベースキャラクター初期化
	BaseCharacter::Initialize(models);
	textureHandle_ = textureHandle;

	// 初期座標
	worldTransform_.translation_ = {0.0f, 50.0f, 300.0f};
	worldTransform_.rotation_.y = float(std::numbers::pi);
	worldTransform_.UpdateMatrix();

	// デスフラグ
	isDead_ = false;

	// 速度
	velocity_ = {
	    0.0f,
	    0.0f,
	    0.0f,
	};

	// エネミーの数
	enemiesJoiningNum = 0;

	// 攻撃準備
	// クールタイム
	preAttackT_ = 0.0f;

	//	回転用のT
	preAttackCooltime_ = preAttackCooltimeMax_;

	// 体力
	hp_ = hp;

	// 無敵か
	isInvincible_ = false;

	// 無敵タイマー
	invincibilityTimer_ = 0;

	// 衝突無敵タイマー
	collisionInvincibilityTime_ = 20;

	// 雷弾無敵タイマー
	thunderInvincibilityTime_ = 40;

	// 射撃クールタイム
	shotAttackCooltime_ = 0;

	// 最大射撃クールタイム
	shotAttackCooltimeMax_ = 60;

	// 部隊
	for (size_t i = 0; i < kUnitPattern; i++) {
		// その部隊パターンのエネミーの数
		unitTransformNum_[i] = 1;
		// 座標
		for (size_t j = 0; j < kUnitTransformnumMax; j++) {
			unitTtransformData_[i][j] = Vector3(0.0f, 0.0f, 0.0f);
		}
	}

	// 振動強さ
	shakeStrength_ = {0.0f, 0.0f, 0.0f};
	// シェイク有効トリガー
	enableShake_ = false;
	// シェイク演出用t
	shakeT_ = 0.0f;
	// シェイク演出時間
	shakeTime_ = 0.0f;

	modelWorldTransform_.Initialize();
	modelWorldTransform_.parent_ = &worldTransform_;
	modelWorldTransform_.UpdateMatrix();

	// エネミーマネージャー
	enemyManager_ = enemyManager;
	//エネミー
	enemies_ = enemyManager_->GetEnemiesAddress();
	//プレイヤー
	player_ = player;

	//効果音リスト
	audioHandles_ = audioHandles;

	//オーディオ
	audio_ = Audio::GetInstance();

	// 衝突属性を設定
	SetCollisionAttribute(0xfffffffb);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(0x00000004);

	// タグ
	tag_ = TagBossEnemy;

	// コライダーの形
	OBB* obb = new OBB();
	obb->Initialize(GetWorldPosition(), worldTransform_.rotation_, Vector3(100.0f,100.0f,100.0f));
	colliderShape_ = obb;

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "BossEnemy";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	globalVariables->AddItem(groupName, "MoveSpeed", moveSpeed_);
	globalVariables->AddItem(groupName, "MoveRotateSpeed", moveRotateSpeed_);
	globalVariables->AddItem(groupName, "BulletSpeed", bulletSpeed_);
	globalVariables->AddItem(
	    groupName, "CollisionInvincibilityTimer", int(collisionInvincibilityTime_));
	globalVariables->AddItem(groupName, "ThunderInvincibilityTime", int(thunderInvincibilityTime_));


	// グループ名設定
	const char* groupName2 = "BossEnemyUnit";
	for (size_t i = 0; i < kUnitPattern; i++) {
		// その部隊パターンのエネミーの数
		char str[32];
		sprintf_s(str, "UnitTransformNum%d", int(i));
		globalVariables->AddItem(groupName2, str, int(unitTransformNum_[i]));
		// 座標
		for (size_t j = 0; j < kUnitTransformnumMax; j++) {
			sprintf_s(str, "UnitTtransformData%d_%d", int(i),int(j));
			globalVariables->AddItem(groupName2, str, unitTtransformData_[i][j]);
		}
	}

	// メンバ変数の調整したい項目をグローバル変数に追加
	colliderShape_->AddToGlobalVariables(groupName);

	ApplyGlobalVariables();

	// どのパターンか
	unitPattern_ = 0;
	// エネミーの最大数
	enemiesJoiningNumMax = unitTransformNum_[unitPattern_];
	for (size_t j = 0; j < kUnitTransformnumMax; j++) {
		unitTtransform_[j] = unitTtransformData_[unitPattern_][j];
	}

}

/// <summary>
/// 更新
/// </summary>
void BossEnemy::Update(std::list<Enemy*>* enemies) {

	ApplyGlobalVariables();

	enemies_ = enemies;

	// 前フレームの位置を保存
	preTranslation_ = worldTransform_.translation_;
	
	// 無敵タイマー処理
	if (isInvincible_) {
		if (--invincibilityTimer_ == 0) {
			isInvincible_ = false;
		}
	}
	
	switch (bossEnemyState_) {
	case BossEnemy::Collect:
		// エネミーの収集
		CollectEnemies();
		break;
	case BossEnemy::PreAttackCommand:
		PreAttack();
		break;
	case BossEnemy::AttackCommand:
		//攻撃中
		UnderAttack();
		break;
	case BossEnemy::Down:
		DownMove();
		break;
	default:
		break;
	}

	//位置制限
	TranslationLimit();

	// ワールド行列更新
	BaseCharacter::Update();

	// コライダー更新
	colliderShape_->Update(GetWorldPosition(), worldTransform_.rotation_, colliderShape_->GetSize());

	// モデルトランスフォーム更新
	// シェイク
	if (enableShake_) {
		modelWorldTransform_.translation_ = ModelShake();
	} else {
		modelWorldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	}
	modelWorldTransform_.UpdateMatrix();

	// サンダーと当たったかをfalseに
	isCollisionThunder = false;

}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
void BossEnemy::Draw(const ViewProjection& viewProjection) {

	for (Model* model : models_) {
		model->Draw(modelWorldTransform_, viewProjection);
	}
}

// 衝突時に呼ばれる関数
void BossEnemy::OnCollision(Collider* collision) { 

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
		CollisionBulletIce(collision);
		break;
	case TagPlayerBulletThunder:
		CollisionBulletThunder();
		break;
	case TagEnemy:
		if (collision->GetEnemy()->GetEnemyState() != Enemy::Appear) {
			CollisionEnemy();
		}
		break;
	default:
		break;
	}

}

void BossEnemy::Move() {

	// 回転
	// 回転速度
	Vector3 velocity(0.0f, moveRotateSpeed_, 0.0f);
	worldTransform_.rotation_ = worldTransform_.rotation_ + velocity;

	// 移動
	//  速度ベクトルを自機の向きに合わせて回転させる
	velocity = {0.0f, 0.0f, moveSpeed_};

	velocity_ = MyMath::Transform(velocity, MyMath::MakeRotateXYZMatrix(worldTransform_.rotation_));

	if (isCollisionThunder) {
		velocity_ = velocity_ * decelerationMagnification;
	}

}

void BossEnemy::CollectEnemies() {

	//Wait状態の一番近いエネミーを検索

	//検索出来たか
	bool enemyFound = false;
	Vector3 shortestPos = {0.0f, 0.0f, 0.0f};
	float shortest = 0.0f;
	// ワールド座標を取得する
	Vector3 pos = GetWorldPosition();
	for (Enemy* enemy : *enemies_) {
		if (enemy) {
			if (enemy->GetEnemyState() == Enemy::Wait) {
				// ボスのワールド座標を取得する
				Vector3 enemyPos = enemy->GetWorldPosition();
				// 敵弾->自キャラの差分ベクトルを求める
				Vector3 toEnemy = pos - enemyPos;
				// 距離
				float distance = MyMath::Length(toEnemy);
				if (enemyFound == false || shortest >= distance) {
					shortest = distance;
					shortestPos = enemyPos;
					enemyFound = true;
				}
			}
		}
	}

	if (enemyFound) {
		//移動割合
		Vector3 toEnemy = MyMath::Normalize(shortestPos - pos);
		// 速度
		velocity_ = toEnemy * moveSpeed_;
		//回転
		MoveRotation(toEnemy);
	} else {
		//通常移動
		Move();
	}

	// 氷の跳ね返り
	velocity_ = velocity_ + accelerationIce_;
	if (accelerationIce_.x != 0.0f) {
		Vector3 preAccel = accelerationIce_;
		accelerationIce_ = accelerationIce_ + accelerationIceDown_;
		if (accelerationIce_.x * preAccel.x < 0.0f || accelerationIce_.y * preAccel.y < 0.0f ||
		    accelerationIce_.z * preAccel.z < 0.0f) {
			accelerationIce_.x = 0.0f;
			accelerationIce_.y = 0.0f;
			accelerationIce_.z = 0.0f;
		}
	}

	//雷の減速
	if (isCollisionThunder) {
		velocity_ = velocity_ * decelerationMagnification;
	}

	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;

	//射撃
	//プレイヤーと向きあっている
	//  自キャラのワールド座標を取得する
	Vector3 playerPos = player_->GetWorldPosition();
	// 敵のワールド座標を取得する
	Vector3 bossEnemyrPos = GetWorldPosition();
	// 敵->自キャラの差分ベクトルを求める
	//Vector3 toPlayer = playerPos - bossEnemyrPos;
	//プレイヤーのレイ
	Vector3 ray = MyMath::Transform(
	                  Vector3(0.0f, 0.0f, 1.0f),
	                  MyMath::MakeRotateXYZMatrix(player_->GetWorldTransform()->rotation_)) +
	              playerPos;
	Vector3 playerRay = playerPos - ray;

	// ボスのレイ
	ray = MyMath::Transform(
	                  Vector3(0.0f, 0.0f, 1.0f),
	                  MyMath::MakeRotateXYZMatrix(worldTransform_.rotation_)) +
	      bossEnemyrPos;
	Vector3 enemyRay = bossEnemyrPos - ray;

	Vector3 toPlayer = bossEnemyrPos - playerPos;

	//向きあっているかの値
	float faceToFace = -0.5f;

	if (MyMath::Dot(enemyRay, playerRay) <= faceToFace && 
		MyMath::Dot(enemyRay, toPlayer) >= 0.0f) {
		if (shotAttackCooltime_ > 0) {
			--shotAttackCooltime_;
		} else {
			shotAttackCooltime_ = shotAttackCooltimeMax_;
			ShotAttack();
		}
	}

}

void BossEnemy::PreAttack() {

	Vector3 rotate = worldTransform_.rotation_;
	Vector3 target = worldTransform_.rotation_;
	//イージング回転
	if (preAttackT_ < 1.0f) {
		float pi = float(std::numbers::pi);

		// 回転
		// 自キャラのワールド座標を取得する
		Vector3 playerPos = player_->GetWorldPosition();
		// 敵のワールド座標を取得する
		Vector3 bossEnemyrPos = GetWorldPosition();
		// 敵->自キャラの差分ベクトルを求める
		Vector3 toPlayer = playerPos - bossEnemyrPos;

		//  Y軸周りの角度(Θy)
		target.y = std::atan2f(toPlayer.x, toPlayer.z);
		// 横軸方向の長さを求める
		float length = MyMath::Length(Vector3{toPlayer.x, 0.0f, toPlayer.z});
		// X軸周りの角度(Θx)
		target.x = std::atan2f(-toPlayer.y, length);

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
		preAttackT_ += preAttackTSpeed_;
		if (preAttackT_ >= 1.0f) {
			preAttackT_ = 1.0f;
		}
		worldTransform_.rotation_ = {
		    MyMath::Linear(preAttackT_, rotate.x, target.x),
		    MyMath::Linear(preAttackT_, rotate.y, target.y),
		    rotate.z
		};
	}
	//イージング終了したら待機
	else {
		if (--preAttackCooltime_ == 0) {
			// 攻撃開始
			BeginAttack();
			preAttackCooltime_ = preAttackCooltimeMax_;
			preAttackT_ = 0.0f;
		}
		// 回転
		// 自キャラのワールド座標を取得する
		Vector3 playerPos = player_->GetWorldPosition();
		// 敵のワールド座標を取得する
		Vector3 bossEnemyrPos = GetWorldPosition();
		// 敵->自キャラの差分ベクトルを求める
		Vector3 toPlayer = playerPos - bossEnemyrPos;

		//  Y軸周りの角度(Θy)
		target.y = std::atan2f(toPlayer.x, toPlayer.z);
		// 横軸方向の長さを求める
		float length = MyMath::Length(Vector3{toPlayer.x, 0.0f, toPlayer.z});
		// X軸周りの角度(Θx)
		target.x = std::atan2f(-toPlayer.y, length);

		worldTransform_.rotation_ = {
		    MyMath::Linear(preAttackT_, rotate.x, target.x),
		    MyMath::Linear(preAttackT_, rotate.y, target.y), rotate.z};

	}

}

void BossEnemy::BeginAttack() {

	bossEnemyState_ = AttackCommand; 

	//クールタイム
	attackCooltime_ = attackCooltimeMax_;

	// 回転
	// 自キャラのワールド座標を取得する
	Vector3 playerPos = player_->GetWorldPosition();
	// 敵弾のワールド座標を取得する
	Vector3 bossEnemyrPos = GetWorldPosition();
	// 敵弾->自キャラの差分ベクトルを求める
	Vector3 toPlayer = playerPos - bossEnemyrPos;
	//  Y軸周りの角度(Θy)
	worldTransform_.rotation_.y = std::atan2f(toPlayer.x, toPlayer.z);
	// 横軸方向の長さを求める
	float length = MyMath::Length(Vector3{toPlayer.x, 0.0f, toPlayer.z});
	// X軸周りの角度(Θx)
	worldTransform_.rotation_.x = std::atan2f(-toPlayer.y, length);

}

void BossEnemy::UnderAttack() {

	if (--attackCooltime_ == 0) {
		bossEnemyState_ = Collect;
		enemiesJoiningNum = 0;
		//ユニットパターン選択
		unitPattern_ = 0;
		//追従エネミー数の指定
		enemiesJoiningNumMax = unitTransformNum_[unitPattern_];
		//トランスフォーム設定
		for (size_t j = 0; j < kUnitTransformnumMax; j++) {
			unitTtransform_[j] = unitTtransformData_[unitPattern_][j];
		}
	} else {
		// 回転
		// 自キャラのワールド座標を取得する
		Vector3 playerPos = player_->GetWorldPosition();
		// 敵弾のワールド座標を取得する
		Vector3 bossEnemyrPos = GetWorldPosition();
		// 敵弾->自キャラの差分ベクトルを求める
		Vector3 toPlayer = playerPos - bossEnemyrPos;
		//  Y軸周りの角度(Θy)
		worldTransform_.rotation_.y = std::atan2f(toPlayer.x, toPlayer.z);
		// 横軸方向の長さを求める
		float length = MyMath::Length(Vector3{toPlayer.x, 0.0f, toPlayer.z});
		// X軸周りの角度(Θx)
		worldTransform_.rotation_.x = std::atan2f(-toPlayer.y, length);
	}

}

void BossEnemy::MoveRotation(Vector3 toPosition) {

	Vector3 rotate = worldTransform_.rotation_;
	Vector3 target = worldTransform_.rotation_;
	float pi = float(std::numbers::pi);

	//  Y軸周りの角度(Θy)
	target.y = std::atan2f(toPosition.x, toPosition.z);
	// 横軸方向の長さを求める
	float length = MyMath::Length(Vector3{toPosition.x, 0.0f, toPosition.z});
	// X軸周りの角度(Θx)
	target.x = std::atan2f(-toPosition.y, length);

	//ターゲット
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
	    MyMath::Linear(t, rotate.x, target.x), MyMath::Linear(t, rotate.y, target.y), rotate.z
	};

}

void BossEnemy::ShotAttack() {

	// 弾の速度
	Vector3 velocity(0, 0, bulletSpeed_);
	// 弾の位置
	Vector3 position = GetWorldPosition();
	//距離
	float distance = 45.0f;

	position.z += std::cosf(worldTransform_.rotation_.y) * std::cosf(worldTransform_.rotation_.x) *
	              distance; // コサイン
	position.x +=
	    std::sinf(worldTransform_.rotation_.y) * std::cosf(worldTransform_.rotation_.x) * distance;
	position.y +=
	    -std::sinf(worldTransform_.rotation_.x) * std::cosf(worldTransform_.rotation_.z) * distance;


	//回転
	Vector3 rotate = {
	    0.0f,
	    0.0f,
	    0.0f
	};
	// 自キャラのワールド座標を取得する
	Vector3 playerPos = player_->GetWorldPosition();
	// 敵弾のワールド座標を取得する
	Vector3 bossEnemyrPos = GetWorldPosition();
	// 敵弾->自キャラの差分ベクトルを求める
	Vector3 toPlayer = playerPos - bossEnemyrPos;
	//  Y軸周りの角度(Θy)
	rotate.y = std::atan2f(toPlayer.x, toPlayer.z);
	// 横軸方向の長さを求める
	float length = MyMath::Length(Vector3{toPlayer.x, 0.0f, toPlayer.z});
	// X軸周りの角度(Θx)
	rotate.x = std::atan2f(-toPlayer.y, length);
	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = MyMath::Transform(velocity, MyMath::MakeRotateXYZMatrix(rotate));
	enemyManager_->AddEnemyBullet(position, velocity);

}

void BossEnemy::Dead() {

	bossEnemyState_ = Down;
	
	audio_->PlayWave(audioHandles_[Audio::BossEnemyDeath]);

}

void BossEnemy::HpFluctuation(int32_t damage, uint32_t InvincibilityTime) {

	if (isInvincible_) {
		return;
	}

	hp_ -= damage;
	if (hp_ <= 0) {
		Dead();
	} else {
		isInvincible_ = true;
		invincibilityTimer_ = InvincibilityTime;
		// シェイク
		PlayModelShake(Vector3(3.0f, 3.0f, 3.0f), float(InvincibilityTime) / 60.0f);

		audio_->PlayWave(audioHandles_[Audio::BossEnemyDamage]);
	}

}

void BossEnemy::TranslationLimit() {

	// トランスフォームの制限確認
	Field* field = Field::GetInstance();
	worldTransform_.translation_.x  = std::clamp(
	    worldTransform_.translation_.x, field->GetMin().x, field->GetMax().x);
	
	worldTransform_.translation_.y =
	    std::clamp(worldTransform_.translation_.y, field->GetMin().y, field->GetMax().y);
	
	worldTransform_.translation_.z =
	    std::clamp(worldTransform_.translation_.z, field->GetMin().z, field->GetMax().z);

}

void BossEnemy::PlayModelShake(Vector3 shakeStrength, float shakeTime) {

	// 引数の値をメンバ変数に代入
	shakeStrength_ = shakeStrength;
	shakeTime_ = shakeTime;

	// 演出用tをリセット
	shakeT_ = 0.0f;
	// カメラ振動有効
	enableShake_ = true;

}

Vector3 BossEnemy::ModelShake() { 

	Vector3 shakeWorldTransformTranslation;
	// 指定秒数シェイク
	if (shakeT_ <= shakeTime_) {
		// シェイクをイージングで表現
		shakeWorldTransformTranslation.x = MyMath::EaseOut(
		    shakeT_, MyMath::RandomF(-shakeStrength_.x, shakeStrength_.x, 2), 0.0f, shakeTime_);
		shakeWorldTransformTranslation.y = MyMath::EaseOut(
		    shakeT_, MyMath::RandomF(-shakeStrength_.y, shakeStrength_.y, 2), 0.0f, shakeTime_);
		shakeWorldTransformTranslation.z = MyMath::EaseOut(
		    shakeT_, MyMath::RandomF(-shakeStrength_.z, shakeStrength_.z, 2), 0.0f, shakeTime_);
		// 演出t加算
		shakeT_ += 1.0f / 60.0f;
	} else {
		// カメラシェイク強さリセット
		shakeWorldTransformTranslation = {0.0f, 0.0f, 0.0f};
		enableShake_ = false;
	}

	return shakeWorldTransformTranslation;

}

void BossEnemy::DownMove() {

	//したに落ちる
	float down = 1.0f;
	worldTransform_.translation_.y -= down;
	//上を向く
	Vector3 rotate = Vector3(float(std::numbers::pi / 2.0f), worldTransform_.rotation_.y, 0.0f);
	MoveRotation(rotate);
	//落ちきったらクリアへ
	if (worldTransform_.translation_.y <= -70.0f) {
		isDead_ = true;
	}

}

void BossEnemy::CollisionBulletNone() {

	HpFluctuation(
	    player_->GetBulletDamage(PlayerBullet::BulletType::None), collisionInvincibilityTime_);

}

void BossEnemy::CollisionBulletFire() {

	HpFluctuation(
	    player_->GetBulletDamage(PlayerBullet::BulletType::Fire), collisionInvincibilityTime_);

}

void BossEnemy::CollisionBulletIce(Collider* collision) {

	if (collision->GetPlayerBullet()->GetIsHit()) {
		if (bossEnemyState_ == Collect) {
			/*
			float reflection = -10.0f;
			float accelerationDown = 1.0f / 2.0f;

			worldTransform_.translation_ = preTranslation_;
			accelerationIce_ = MyMath::Normalize(velocity_) * reflection;
			accelerationIceDown_ = MyMath::Normalize(velocity_) * accelerationDown;
			worldTransform_.UpdateMatrix();
			*/

				Vector3* iceOtientatuons = collision->GetColliderShape()->GetOtientatuons();

			Vector3 size_ = collision->GetColliderShape()->GetSize();

			// 頂点
			Vector3 vertex[8];

			vertex[0] = {
			    (-size_.x * iceOtientatuons[0].x + size_.y * iceOtientatuons[1].x +
			     -size_.z * iceOtientatuons[2].x),
			    (-size_.x * iceOtientatuons[0].y + size_.y * iceOtientatuons[1].y +
			     -size_.z * iceOtientatuons[2].y),
			    (-size_.x * iceOtientatuons[0].z + size_.y * iceOtientatuons[1].z +
			     -size_.z * iceOtientatuons[2].z)};

			vertex[1] = {
			    (-size_.x * iceOtientatuons[0].x + size_.y * iceOtientatuons[1].x +
			     size_.z * iceOtientatuons[2].x),
			    (-size_.x * iceOtientatuons[0].y + size_.y * iceOtientatuons[1].y +
			     size_.z * iceOtientatuons[2].y),
			    (-size_.x * iceOtientatuons[0].z + size_.y * iceOtientatuons[1].z +
			     size_.z * iceOtientatuons[2].z)};

			vertex[2] = {
			    (size_.x * iceOtientatuons[0].x + size_.y * iceOtientatuons[1].x +
			     -size_.z * iceOtientatuons[2].x),
			    (size_.x * iceOtientatuons[0].y + size_.y * iceOtientatuons[1].y +
			     -size_.z * iceOtientatuons[2].y),
			    (size_.x * iceOtientatuons[0].z + size_.y * iceOtientatuons[1].z +
			     -size_.z * iceOtientatuons[2].z)};

			vertex[3] = {
			    (size_.x * iceOtientatuons[0].x + size_.y * iceOtientatuons[1].x +
			     size_.z * iceOtientatuons[2].x),
			    (size_.x * iceOtientatuons[0].y + size_.y * iceOtientatuons[1].y +
			     size_.z * iceOtientatuons[2].y),
			    (size_.x * iceOtientatuons[0].z + size_.y * iceOtientatuons[1].z +
			     size_.z * iceOtientatuons[2].z)};

			vertex[4] = {
			    (-size_.x * iceOtientatuons[0].x + -size_.y * iceOtientatuons[1].x +
			     -size_.z * iceOtientatuons[2].x),
			    (-size_.x * iceOtientatuons[0].y + -size_.y * iceOtientatuons[1].y +
			     -size_.z * iceOtientatuons[2].y),
			    (-size_.x * iceOtientatuons[0].z + -size_.y * iceOtientatuons[1].z +
			     -size_.z * iceOtientatuons[2].z)};

			vertex[5] = {
			    (-size_.x * iceOtientatuons[0].x + -size_.y * iceOtientatuons[1].x +
			     size_.z * iceOtientatuons[2].x),
			    (-size_.x * iceOtientatuons[0].y + -size_.y * iceOtientatuons[1].y +
			     size_.z * iceOtientatuons[2].y),
			    (-size_.x * iceOtientatuons[0].z + -size_.y * iceOtientatuons[1].z +
			     size_.z * iceOtientatuons[2].z)};

			vertex[6] = {
			    (size_.x * iceOtientatuons[0].x + -size_.y * iceOtientatuons[1].x +
			     -size_.z * iceOtientatuons[2].x),
			    (size_.x * iceOtientatuons[0].y + -size_.y * iceOtientatuons[1].y +
			     -size_.z * iceOtientatuons[2].y),
			    (size_.x * iceOtientatuons[0].z + -size_.y * iceOtientatuons[1].z +
			     -size_.z * iceOtientatuons[2].z)};

			vertex[7] = {
			    (size_.x * iceOtientatuons[0].x + -size_.y * iceOtientatuons[1].x +
			     size_.z * iceOtientatuons[2].x),
			    (size_.x * iceOtientatuons[0].y + -size_.y * iceOtientatuons[1].y +
			     size_.z * iceOtientatuons[2].y),
			    (size_.x * iceOtientatuons[0].z + -size_.y * iceOtientatuons[1].z +
			     size_.z * iceOtientatuons[2].z)};

			Vector3 useVertex[6][4]{
			    {vertex[2], vertex[3], vertex[7], vertex[6]},
			    {vertex[1], vertex[0], vertex[4], vertex[5]},
			    {vertex[0], vertex[1], vertex[3], vertex[2]},
			    {vertex[6], vertex[7], vertex[5], vertex[4]},
			    {vertex[5], vertex[7], vertex[3], vertex[1]},
			    {vertex[0], vertex[2], vertex[6], vertex[4]},
			};

			// 氷から平面
			Vector3 icePlaneNormal[6] = {};
			icePlaneNormal[0] = iceOtientatuons[0];
			icePlaneNormal[1] = iceOtientatuons[0] * -1;
			icePlaneNormal[2] = iceOtientatuons[1];
			icePlaneNormal[3] = iceOtientatuons[1] * -1;
			icePlaneNormal[4] = iceOtientatuons[2];
			icePlaneNormal[5] = iceOtientatuons[2] * -1;
			float iceDistance[6] = {};
			iceDistance[0] = collision->GetColliderShape()->GetSize().x;
			iceDistance[1] = collision->GetColliderShape()->GetSize().x;
			iceDistance[2] = collision->GetColliderShape()->GetSize().y;
			iceDistance[3] = collision->GetColliderShape()->GetSize().y;
			iceDistance[4] = collision->GetColliderShape()->GetSize().z;
			iceDistance[5] = collision->GetColliderShape()->GetSize().z;

			// 線分作成
			Vector3 origin = {0.0f, 0.0f, 0.0f};
			Vector3 diff = colliderShape_->GetCenter() - collision->GetColliderShape()->GetCenter();
			size_t num = 0;

			for (size_t i = 0; i < 6; i++) {

				// 垂直判定のため、法線と線の内積を求める
				float dot = MyMath::Dot(icePlaneNormal[i], diff);

				// 衝突しているかも
				if (dot != 0.0f) {

					// tを求める
					float t = (iceDistance[i] - MyMath::Dot(origin, icePlaneNormal[i])) / dot;

					if (t >= 0) {

						// 平面の中か
						Vector3 v01 = useVertex[i][1] - useVertex[i][0];
						Vector3 v12 = useVertex[i][2] - useVertex[i][1];
						Vector3 v23 = useVertex[i][3] - useVertex[i][2];
						Vector3 v30 = useVertex[i][0] - useVertex[i][3];

						Vector3 v1p = origin + (diff * t) - useVertex[i][1];
						Vector3 v2p = origin + (diff * t) - useVertex[i][2];
						Vector3 v3p = origin + (diff * t) - useVertex[i][3];
						Vector3 v0p = origin + (diff * t) - useVertex[i][0];

						Vector3 cross01 = MyMath::Cross(v01, v1p);
						Vector3 cross12 = MyMath::Cross(v12, v2p);
						Vector3 cross23 = MyMath::Cross(v23, v3p);
						Vector3 cross30 = MyMath::Cross(v30, v0p);

						if (MyMath::Dot(cross01, icePlaneNormal[i]) >= 0.0f &&
						    MyMath::Dot(cross12, icePlaneNormal[i]) >= 0.0f &&
						    MyMath::Dot(cross23, icePlaneNormal[i]) >= 0.0f &&
						    MyMath::Dot(cross30, icePlaneNormal[i]) >= 0.0f) {

							num = i;
							break;
						}
					}
				}
			}

			// 平面とObb
			float r = 0.0f;
			Vector3* playerOtientatuons = collision->GetColliderShape()->GetOtientatuons();
			float playersize[3] = {
			    colliderShape_->GetSize().x,
			    colliderShape_->GetSize().y,
			    colliderShape_->GetSize().z,
			};
			for (size_t i = 0; i < 3; i++) {
				r += std::fabsf(
				    MyMath::Dot(playerOtientatuons[i] * playersize[i], icePlaneNormal[num]));
			}

			// 平面とobbの距離
			Vector3 playerPos = colliderShape_->GetCenter();
			Vector3 planePos = collision->GetColliderShape()->GetCenter() +
			                   (icePlaneNormal[num] * iceDistance[num]);

			float s = MyMath::Dot(playerPos - planePos, icePlaneNormal[num]);
			float distance = 0.0f;
			if (s > 0) {
				distance = r - std::fabsf(s);
			} else {
				distance = r + std::fabsf(s);
			}

			worldTransform_.translation_ =
			    worldTransform_.translation_ + (icePlaneNormal[num] * distance);


		}
	} else {
		HpFluctuation(
		    player_->GetBulletDamage(PlayerBullet::BulletType::Ice), collisionInvincibilityTime_);
	}

}

void BossEnemy::CollisionBulletThunder() {

	HpFluctuation(
	    player_->GetBulletDamage(PlayerBullet::BulletType::Thunder), thunderInvincibilityTime_);

	isCollisionThunder = true;

}

void BossEnemy::CollisionEnemy() {
	
	if (bossEnemyState_ == Collect) {
		enemiesJoiningNum++;
		if (enemiesJoiningNum == enemiesJoiningNumMax) {
			bossEnemyState_ = PreAttackCommand;
		}
	}

}

void BossEnemy::CollisionPlayer() {}

void BossEnemy::ApplyGlobalVariables() {

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "BossEnemy";

	// メンバ変数の調整項目をグローバル変数に追加
	moveSpeed_ = globalVariables->GetFloatValue(groupName, "MoveSpeed");
	moveRotateSpeed_ = globalVariables->GetFloatValue(groupName, "MoveRotateSpeed");
	bulletSpeed_ = globalVariables->GetFloatValue(groupName, "BulletSpeed");
	collisionInvincibilityTime_ =
	    uint32_t(globalVariables->GetIntValue(groupName, "CollisionInvincibilityTimer"));
	thunderInvincibilityTime_ =
	    uint32_t(globalVariables->GetIntValue(groupName, "ThunderInvincibilityTime"));


	// グループ名設定
	const char* groupName2 = "BossEnemyUnit";
	for (size_t i = 0; i < kUnitPattern; i++) {
		// その部隊パターンのエネミーの数
		char str[32];
		sprintf_s(str, "UnitTransformNum%d", int(i));
		unitTransformNum_[i] = uint32_t(globalVariables->GetIntValue(groupName2, str));
		// 座標
		for (size_t j = 0; j < kUnitTransformnumMax; j++) {
			sprintf_s(str, "UnitTtransformData%d_%d", int(i), int(j));
			unitTtransformData_[i][j] = globalVariables->GetVector3Value(groupName2, str);	
		}
	}

	colliderShape_->ApplyGlobalVariables(groupName);

}
