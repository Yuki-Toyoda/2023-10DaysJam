#include "PlayerBullet.h"

void PlayerBullet::Initialize(
    const std::vector<Model*>& models, Vector3 startPos, Vector3 startotation, Vector3 velocity) {
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
	bulletType_ = None;

	// 生存している状態に
	isDead_ = false;
	// 生存時間設定
	deathTimer_ = klifeTime_;
}

void PlayerBullet::Initialize(
    const std::vector<Model*>& models, Vector3 startPos, Vector3 startotation, Vector3 velocity,
    BulletType bulletType, int32_t bulletStrength) {
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
	bulletStrength_ = bulletStrength;

	switch (bulletType_) {
	case PlayerBullet::Fire:
		// 何とも衝突していない
		isHit_ = false;
		// 落下速度初期化
		fallSpeed_ = 0.0f;
		// 弾道落下スピード最大値設定
		kMaxFallSpeed_ = 3.0f;
		// 落下加速度初期化
		kFallAcceleration_ = 0.00098f;
		// 爆破範囲設定
		explosiveRange_ = {
		    50.0f * (1.0f + 0.35f * bulletStrength), 
			50.0f * (1.0f + 0.35f * bulletStrength),
		    50.0f * (1.0f + 0.35f * bulletStrength)};
		// 爆破演出用t
		explosiveT_ = 0.0f;
		// 爆破演出時間
		explosiveTime_ = 0.5f;
		break;
	case PlayerBullet::Water:
		break;
	case PlayerBullet::Thunder:
		break;
	}

	// 生存している状態に
	isDead_ = false;
	// 生存時間設定
	deathTimer_ = klifeTime_;
}

void PlayerBullet::Update() {
	// 弾の種類によって更新処理変化
	switch (bulletType_) {
	case PlayerBullet::None:
		// 通常弾更新
		NormalBulletUpdate();
		break;
	case PlayerBullet::Fire:
		// 炎弾更新
		FireBulletUpdate();
		break;
	case PlayerBullet::Water:
		// 水弾更新
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
	switch (bulletType_) {
	case PlayerBullet::None:
		models_[0]->Draw(worldTransform_, viewProjection);
		break;
	case PlayerBullet::Fire:
		models_[1]->Draw(worldTransform_, viewProjection);
		break;
	case PlayerBullet::Water:
		models_[2]->Draw(worldTransform_, viewProjection);
		break;
	case PlayerBullet::Thunder:
		models_[3]->Draw(worldTransform_, viewProjection);
		break;
	default:
		break;
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
	// 何かと衝突していなければ
	if (!isHit_) {
		// 弾をベクトルの方向に前進させる
		worldTransform_.translation_ = worldTransform_.translation_ + velocity_;

		// １フレーム落下した後の座標を一時的に計算
		float tempTranslateY = worldTransform_.translation_.y + fallSpeed_;
		// 一時座標が床以下になったら
		if (tempTranslateY <= 0.0f) {
			// 座標を床位置にリセット
			worldTransform_.translation_.y = 0.0f;
			// 落下速度初期化
			fallSpeed_ = 0.0f;
			// 床と衝突
			isHit_ = true;
		} else {
			// 落下スピード加算
			velocity_.y += fallSpeed_;
			if (fallSpeed_ <= kMaxFallSpeed_) {
				fallSpeed_ -= kFallAcceleration_;
			} else {
				fallSpeed_ = kMaxFallSpeed_;
			}
			
		}
	} else {
		// 爆破演出をイージングで行う
		if (explosiveT_ <= explosiveTime_) {
			worldTransform_.scale_ = 
				MyMath::EaseOut(explosiveT_, {0.5f, 0.5f, 0.5f}, explosiveRange_, explosiveTime_);
			// 演出用tを加算
			explosiveT_ += 1.0f / 60.0f;
		} else {
			// 弾を消去する
			isDead_ = true;
		}
	}
}

void PlayerBullet::WaterBulletUpdate() {

}

void PlayerBullet::ThunderBulletUpdate() {

}

void PlayerBullet::OnCollision() {

}
