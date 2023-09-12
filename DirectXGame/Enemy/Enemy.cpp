#include "Enemy.h"
#include "EnemyBullet.h"
#include "EnemyManager.h"
#include <cassert>
#include "MyMath.h"
#include "Charactor/CharactorList.h"
#include "../config/GlobalVariables.h"
#include "Collision/ColliderShape/OBB.h"
#include "Ambient/Field.h"
#include <Effect/EffectManager.h>

/// <summary>
/// 初期化
/// </summary>
/// <param name="models">モデルデータ配列</param>
void Enemy::Initialize(const std::vector<Model*>& models, uint32_t textureHandle, EnemyType enemyType,
    Vector3 posioton, uint32_t hp, EnemyManager* enemyManager, Player* player,
    std::list<BossEnemy*>* bossEnemies, const std::vector<Model*>& deathEffectModels,
    bool isTutorial, const ViewProjection* viewProjection) {
	
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
	enemyState_ = Appear;

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

	//雷弾無敵タイム
	thunderInvincibilityTime_ = 40;

	// 振動強さ
	shakeStrength_ = {0.0f,0.0f,0.0f};
	// シェイク有効トリガー
	enableShake_ = false;
	// シェイク演出用t
	shakeT_ = 0.0f;
	// シェイク演出時間
	shakeTime_ = 0.0f;

	modelWorldTransform_.Initialize();
	modelWorldTransform_.parent_ = &worldTransform_;
	modelWorldTransform_.UpdateMatrix();

	// 敵マーカ用トランスフォームリセット
	worldTransformEnemyMark_.Initialize();
	worldTransformEnemyMark_.translation_ = modelWorldTransform_.translation_;
	worldTransformEnemyMark_.translation_.y += 1.0f;

	// タイマー
	appearTimer_ = appearTime_;

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
	// エネミー死亡エフェクトのモデル
	deathEffectModels_ = deathEffectModels;
	// チュートリアルか
	isTutorial_ = isTutorial;
	// ビュープロジェクションを受け取る
	viewProjection_ = viewProjection;

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
	globalVariables->AddItem(
	    groupName, "ThunderInvincibilityTime", int(thunderInvincibilityTime_));

	colliderShape_->AddToGlobalVariables(groupName);

}

/// <summary>
/// 更新
/// </summary>
void Enemy::Update() {

	switch (enemyState_) {
	case Enemy::Appear:
		Appearing();
		break;
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
		break;
	case Enemy::Tutorial:
		TutorialMove();
		break;
	default:
		break;
	}

	// 前フレームの位置を保存
	preTranslation_ = worldTransform_.translation_;

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

	//モデルトランスフォーム更新
	// シェイク
	if (enableShake_) {
		modelWorldTransform_.translation_ = ModelShake();
	} else {
		modelWorldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	}
	modelWorldTransform_.UpdateMatrix();

	//サンダーと当たったかをfalseに
	isCollisionThunder = false;

	if (isTutorial_) {

		worldTransformEnemyMark_.translation_ = worldTransform_.translation_;
		worldTransformEnemyMark_.translation_.y += 25.0f;

		// ビルボード処理
		// ビュー行列を取得
		Matrix4x4 viewMatrix = MyMath::Inverse(viewProjection_->matView);
		// ビュー行列の移動要素を消す
		viewMatrix.m[3][0] = 0.0f;
		viewMatrix.m[3][1] = 0.0f;
		viewMatrix.m[3][2] = 0.0f;

		// ワールド行列の計算
		Matrix4x4 worldMatrix = MyMath::MakeAffineMatrix(
		    worldTransformEnemyMark_.scale_, worldTransformEnemyMark_.rotation_,
		    worldTransformEnemyMark_.translation_);
		worldTransformEnemyMark_.matWorld_ = viewMatrix * worldMatrix;

		// ワールド座標の転送
		worldTransformEnemyMark_.TransferMatrix();
	}

}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
void Enemy::Draw(const ViewProjection& viewProjection) {
	if (isTutorial_) {
		models_[1]->Draw(worldTransformEnemyMark_, viewProjection, textureHandle_);
	}
	models_[0]->Draw(modelWorldTransform_, viewProjection, textureHandle_);
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
		CollisionBulletIce(collision);
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

	worldTransform_.rotation_.y = worldTransform_.rotation_.y + velocity.y;
	worldTransform_.rotation_.x = 0.0f;
	
	//移動
	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = {0.0f, 0.0f, moveSpeed_};
	if (worldTransform_.translation_.y <= risingHeight) {
		velocity.y = moveSpeed_ / 10.0f;
	}

	velocity_ = MyMath::Transform(velocity, MyMath::MakeRotateXYZMatrix(worldTransform_.rotation_));

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
	velocity_ = toPlayer * moveToPlayerSpeed_;
	//回転
	MoveRotation(toPlayer);

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

void Enemy::Appearing() {

	if (--appearTimer_ == 0) {
		if (isTutorial_) {
			enemyState_ = Tutorial;
		} else {
			enemyState_ = Wait;
		}
	}

	float t = float(appearTime_ - appearTimer_) /  float(appearTime_);
	worldTransform_.scale_.x = MyMath::EaseOut(t, 0.0f, 1.0f);
	worldTransform_.scale_.y = MyMath::EaseOut(t, 0.0f, 1.0f);
	worldTransform_.scale_.z = MyMath::EaseOut(t, 0.0f, 1.0f);

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

	
	// 雷の減速
	if (isCollisionThunder) {
		velocity_ = velocity_ * decelerationMagnification;
	}

	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;

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
	EffectManager::GetInstance()->PlayEnemyDeathEffect(
	    deathEffectModels_, textureHandle_, worldTransform_.translation_);

}

void Enemy::HpFluctuation(int32_t damage, uint32_t InvincibilityTime) {

	if (isInvincible_) {
		return;
	}

	hp_ -= damage;
	if (hp_ <= 0) {
		Dead();
	} else {
		//無敵
		isInvincible_ = true;
		invincibilityTimer_ = InvincibilityTime;
		//シェイク
		PlayModelShake(Vector3(3.0f, 3.0f, 3.0f), float(InvincibilityTime) / 60.0f);
	}

}

void Enemy::PlayModelShake(Vector3 shakeStrength, float shakeTime) {
	// 引数の値をメンバ変数に代入
	shakeStrength_ = shakeStrength;
	shakeTime_ = shakeTime;

	// 演出用tをリセット
	shakeT_ = 0.0f;
	// カメラ振動有効
	enableShake_ = true;

}

Vector3 Enemy::ModelShake() {

	Vector3 shakeWorldTransformTranslation;
	// 指定秒数シェイク
	if (shakeT_ <= shakeTime_) {
		// シェイクをイージングで表現
		shakeWorldTransformTranslation.x = MyMath::EaseOut(shakeT_, MyMath::RandomF(-shakeStrength_.x, shakeStrength_.x, 2), 0.0f, shakeTime_);
		shakeWorldTransformTranslation.y = MyMath::EaseOut(shakeT_, MyMath::RandomF(-shakeStrength_.y, shakeStrength_.y, 2), 0.0f, shakeTime_);
		shakeWorldTransformTranslation.z = MyMath::EaseOut(shakeT_, MyMath::RandomF(-shakeStrength_.z, shakeStrength_.z, 2), 0.0f, shakeTime_);
		// 演出t加算
		shakeT_ += 1.0f / 60.0f;
	} else {
		// カメラシェイク強さリセット
		shakeWorldTransformTranslation = {0.0f, 0.0f, 0.0f};
		enableShake_ = false;
	}

	return shakeWorldTransformTranslation;

}

void Enemy::TutorialMove() {

	// 移動固定
	Move();

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

	// 雷の減速
	if (isCollisionThunder) {
		velocity_ = velocity_ * decelerationMagnification;
	}

	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;


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
		player_->SubtractNeedChangeOrbEnemyCount_();
	}

}

void Enemy::CollisionBulletFire() {

	HpFluctuation(
	    player_->GetBulletDamage(PlayerBullet::BulletType::Fire), collisionInvincibilityTime_);

	if (isDead_) {
		player_->SubtractNeedChangeOrbEnemyCount_();
	}

}

void Enemy::CollisionBulletIce(Collider* collision) {

	if (collision->GetPlayerBullet()->GetIsHit()) {
		if (enemyState_ == Wait) {
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


		} else {
			Dead();
		}
	} else {
		HpFluctuation(
		    player_->GetBulletDamage(PlayerBullet::BulletType::Ice), collisionInvincibilityTime_);
	}

	
	if (isDead_) {
		player_->SubtractNeedChangeOrbEnemyCount_();
	}

}

void Enemy::CollisionBulletThunder() {

	HpFluctuation(
	    player_->GetBulletDamage(PlayerBullet::BulletType::Thunder), thunderInvincibilityTime_);

	isCollisionThunder = true;
	
	if (isDead_) {
		player_->SubtractNeedChangeOrbEnemyCount_();
	}


}

void Enemy::CollisionBossEnemy(BossEnemy* bossEnemy) {

	if (enemyState_ == Wait && bossEnemy->GetBossEnemyState() == BossEnemy::Collect) {
		Join(bossEnemy);
	}

}

void Enemy::CollisionPlayer() {
	if (!isTutorial_) {
		Dead();
	}
}

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
	thunderInvincibilityTime_ =
	    uint32_t(globalVariables->GetIntValue(groupName, "ThunderInvincibilityTime"));

	colliderShape_->ApplyGlobalVariables(groupName);

}
