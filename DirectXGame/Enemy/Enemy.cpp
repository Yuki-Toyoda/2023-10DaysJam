#include "Enemy.h"
#include "EnemyBullet.h"
#include "EnemyManager.h"
#include <cassert>
#include "MyMath.h"
#include "Charactor/CharactorList.h"
#include "../config/GlobalVariables.h"
#include "Collision/ColliderShape/OBB.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="models">モデルデータ配列</param>
void Enemy::Initialize(const std::vector<Model*>& models, uint32_t textureHandle, EnemyType enemyType,
    Vector3 posioton, EnemyManager* enemyManager, Player* player, std::list<BossEnemy*>* bossEnemies) {
	
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

	enemyType_ = enemyType;

	enemyState_ = Wait;

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
	case Enemy::Rush:
		Rushing();
	default:
		break;
	}

	//グローバル変数適用
	ApplyGlobalVariables();

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
void Enemy::OnCollision(Tag collisionTag) {

	if (collisionTag == TagPlayer ||
		collisionTag == TagPlayerBulletFire ||
	    collisionTag == TagPlayerBulletIce ||
		collisionTag == TagPlayerBulletThunder ||
	    collisionTag == TagPlayerBulletNone) {
		isDead_ = true;
	} else if (collisionTag == TagBossEnemy && enemyState_ == Wait) {
		Join();
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
	// 敵弾のワールド座標を取得する
	Vector3 enemyrPos = GetWorldPosition();
	// 敵弾->自キャラの差分ベクトルを求める
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
	// 回転
	MoveRotation(toPlayer);

}

void Enemy::Rushing() {

	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;

}

void Enemy::Waiting() {

	//Move();

}

void Enemy::Following() {

	//最短距離
	Vector3 shortestPos = {0.0f, 0.0f, 0.0f};
	float shortest = 0.0f;
	// ワールド座標を取得する
	Vector3 pos = GetWorldPosition();
	for (BossEnemy* bossEnemy : *bossEnemies_) {
		// ボスのワールド座標を取得する
		Vector3 bossPos = bossEnemy->GetWorldPosition();
		// 敵弾->自キャラの差分ベクトルを求める
		Vector3 toBoss = pos - bossPos;
		//距離
		float distance = MyMath::Length(toBoss);
		if (shortest == 0.0f || shortest >= distance) {
			shortest = distance;	
			shortestPos = bossPos;
		}
	}

	float t = 0.025f;
	worldTransform_.translation_ = {
	    MyMath::Linear(t, pos.x, shortestPos.x),
		MyMath::Linear(t, pos.y, shortestPos.y),
	    MyMath::Linear(t, pos.z, shortestPos.z)
	};

		// 回転
	MoveRotation(shortestPos - pos);

}

void Enemy::Join() {

	enemyState_ = Follow;

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

	colliderShape_->ApplyGlobalVariables(groupName);

}
