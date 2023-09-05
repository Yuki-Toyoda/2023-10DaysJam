#include "BossEnemy.h"
#include "MyMath.h"
#include "Charactor/Player/Player.h"
#include <cassert>

/// <summary>
/// 初期化
/// </summary>
/// <param name="models">モデルデータ配列</param>
void BossEnemy::Initialize(const std::vector<Model*>& models, uint32_t textureHandle) {

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
void BossEnemy::Update() {

	// ワールド行列更新
	worldTransform_.UpdateMatrix();
}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
void BossEnemy::Draw(const ViewProjection& viewProjection) {

	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}
}

// 衝突時に呼ばれる関数
void BossEnemy::OnCollision() { isDead_ = true; }

void BossEnemy::Move() {

	//プレイヤーを向く
	//  自キャラのワールド座標を取得する
	Vector3 playerPos = player_->GetWorldPosition();
	// 敵弾のワールド座標を取得する
	Vector3 enemyrPos = GetWorldPosition();
	// 敵弾->自キャラの差分ベクトルを求める
	Vector3 toPlayer = playerPos - enemyrPos;
	// 距離
	float distance = MyMath::Length(toPlayer);
	// ベクトルの正規化
	toPlayer = MyMath::Normalize(toPlayer);
	// 速度
	velocity_ = toPlayer;

	// 進行方向に見た目の回転を合わせる
	//  Y軸周りの角度(Θy)
	worldTransform_.rotation_.y = std::atan2f(velocity_.x, velocity_.z);
	// 横軸方向の長さを求める
	float length = MyMath::Length(Vector3{velocity_.x, 0.0f, velocity_.z});
	// X軸周りの角度(Θx)
	worldTransform_.rotation_.x = std::atan2f(-velocity_.y, length);

	// 速さ
	float speed;
	if (distance > appropriateDistance + distanceTolerance) {
		// 近づく
		speed = approachThePlayerSpeed;
		// 速度
		velocity_ = velocity_ * speed;
	} else if (distance < appropriateDistance - distanceTolerance) {
		// 遠ざかる
		speed = moveAwayFromPlayerSpeed;
		// 速度
		velocity_ = velocity_ * speed;
	} else {
		// 回転
		speed = playerSatelliteSpeed;
		// 速度
		velocity_ = velocity_ * speed;
		float tmp = velocity_.x;
		velocity_.x = velocity_.z;
		velocity_.z = tmp;
	}

}
