#include "PlayerBullet.h"

void PlayerBullet::Initialize(
    const std::vector<Model*>& models, Vector3 startPos, Vector3 startotation, Vector3 velocity,
    BulletType bulletType) {
	// 基底クラス初期化
	BaseCharacter::Initialize(models);

	// モデル受け取り
	models_ = models;

	// ワールド座標初期化
	worldTransform_.Initialize();

	// 引数の値をメンバ変数に代入
	worldTransform_.translation_ = startPos;
	worldTransform_.rotation_ = startotation;
	velocity_ = velocity;
	bulletType_ = bulletType;

	// 生存している状態に
	isDead_ = false;
	// 生存時間設定
	deathTimer_ = klifeTime_;

	// 衝突属性を設定
	SetCollisionAttribute(0xfffffffe);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(0x00000001);

}

void PlayerBullet::Update() {
	// 弾の種類によって更新処理変化
	switch (bulletType_) {
	case PlayerBullet::Normal:
		// 通常弾更新
		NormalBulletUpdate();
		break;
	case PlayerBullet::Fire:
		// 爆発弾更新
		FireBulletUpdate();
		break;
	case PlayerBullet::Water:
		// ビーム弾更新
		WaterBulletUpdate();
		break;
	case PlayerBullet::Thunder:
		// 雷弾更新
		ThunderBulletUpdate();
		break;
	default:
		break;
	}

	// 基底クラス更新
	BaseCharacter::Update();

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) { 
	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}
}

void PlayerBullet::NormalBulletUpdate() {
	// 弾をベクトルの方向に前進させる
	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;

	// 死亡タイマーデクリメント
	deathTimer_--;
	// 死亡タイマーが0以下になったら
	if (deathTimer_ <= 0) {
		// 弾を死亡させる
		isDead_ = true;
	}
}

void PlayerBullet::FireBulletUpdate() {

}

void PlayerBullet::WaterBulletUpdate() {

}

void PlayerBullet::ThunderBulletUpdate() {

}

void PlayerBullet::OnCollision() {

}
