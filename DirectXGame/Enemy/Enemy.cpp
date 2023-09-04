#include "Enemy.h"
#include "EnemyBullet.h"
#include "EnemyManager.h"
#include <cassert>
#include <MyMath.h>

/// <summary>
/// 初期化
/// </summary>
/// <param name="models">モデルデータ配列</param>
void Enemy::Initialize(const std::vector<Model*>& models, uint32_t textureHandle) {
	
	// NULLポインタチェック
	assert(models.front());

	// ベースキャラクター初期化
	BaseCharacter::Initialize(models);
	textureHandle_ = textureHandle;

	// 初期座標
	worldTransform_.translation_ = {10.0f, 0.0f, 30.0f};
	worldTransform_.UpdateMatrix();

	// デスフラグ
	isDead_ = false;

	// 速度
	velocity_ = {
	    0.0f,
	    0.0f,
	    0.0f,
	};

	// 衝突属性を設定
	SetCollisionAttribute(0xfffffffd);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(0x00000002);

}

/// <summary>
/// 更新
/// </summary>
void Enemy::Update() {

	//ワールド行列更新
	worldTransform_.UpdateMatrix();

}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
void Enemy::Draw(const ViewProjection& viewProjection) {

	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}

}

// 衝突時に呼ばれる関数
void Enemy::OnCollision() {

	isDead_ = true;

}

void Enemy::Move() {



}

void Enemy::Attack() {

	// 弾の速度
	const float kBulletSpeed = 0.5f;
	Vector3 velocity(0, 0, kBulletSpeed);
	// 弾の位置
	Vector3 position = GetWorldPosition();

	position.z += std::cosf(worldTransform_.rotation_.y) * std::cosf(worldTransform_.rotation_.x) *5.0f; // コサイン
	position.x += std::sinf(worldTransform_.rotation_.y) * std::cosf(worldTransform_.rotation_.x) * 5.0f;
	position.y += -std::sinf(worldTransform_.rotation_.x) * std::cosf(worldTransform_.rotation_.z) * 5.0f;

	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = MyMath::Transform(velocity, MyMath::MakeRotateXYZMatrix(worldTransform_.rotation_));
	enemyManager_->AddEnemyBullet(position, velocity);

}

void Enemy::Rush() {

}

void Enemy::Follow() {

}
