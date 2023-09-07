#include "PlayerBullet.h"
#include "Collision/ColliderShape/OBB.h"

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

	// 衝突属性を設定
	SetCollisionAttribute(0xfffffffe);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(0x00000001);

	// コライダーの形
	OBB* obb = new OBB();
	obb->Initialize(GetWorldPosition(), worldTransform_.rotation_, worldTransform_.scale_);
	colliderShape_ = obb;

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

	// 特殊弾用変数初期化
	// 何とも衝突していない
	isHit_ = false;
	// 落下速度初期化
	fallSpeed_ = 0.0f;
	// 弾道落下スピード最大値設定
	kMaxFallSpeed_ = 3.0f;
	// 落下加速度初期化
	kFallAcceleration_ = 0.00098f;
	// 演出用t初期化
	animT_ = 0.0f;
	// 行動中間地点リセット
	actionWayPoint_ = WayPoint1;

	switch (bulletType_) {
	case PlayerBullet::Fire:
		// 爆破範囲設定
		explosiveRange_ = {
		    50.0f * (1.0f + 0.45f * (bulletStrength - 1)), 
			50.0f * (1.0f + 0.45f * (bulletStrength - 1)),
		    50.0f * (1.0f + 0.45f * (bulletStrength - 1))};
		// 爆破演出時間
		explosiveTime_ = 0.25f;
		break;
	case PlayerBullet::Ice:
		// 回転角をリセット
		worldTransform_.rotation_ = {0.0f, worldTransform_.rotation_.y, 0.0f};
		// 壁のサイズ設定
		deployWallSize_ = {
		    50.0f * (1.0f + 0.35f * (bulletStrength - 1)), 
			20.0f * (1.0f + 0.15f * (bulletStrength - 1)), 
			10.0f * (1.0f + 0.15f * (bulletStrength - 1))};
		// 展開演出時間設定
		deployWallStagingTime_ = 0.35f;
		// 展開時間設定
		deploymentWallTime_ = 5.0f * (1.0f + 0.35f * (bulletStrength - 1)), 
		// 終了演出時間設定
		deployWallEndStagingTime_ = 0.15f;
		break;
	case PlayerBullet::Thunder:
		// 回転角をリセット
		worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
		// 展開エリアを設定
		deployAreaSize_ = {
			75.0f * (1.0f + 0.35f * (bulletStrength - 1)),
			10.0f,
		    75.0f * (1.0f + 0.35f * (bulletStrength - 1)),
		};
		// 振動範囲を設定
		shakeRange_ = 1.0f * (1.0f + 0.35f * (bulletStrength - 1));
		// 展開演出時間
		deployAreaStagingTime_ = 0.25f;
		// 展開時間
		deploymentAreaTime_ = 7.0f * (1.0f + 0.35f * (bulletStrength - 1));
		// 終了演出時間
		deployAreaEndStagingTime_ = 0.15f;
		break;
	}

	// 生存している状態に
	isDead_ = false;
	// 生存時間設定
	deathTimer_ = klifeTime_;

	// 衝突属性を設定
	SetCollisionAttribute(0xfffffffe);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(0x00000001);

	// コライダーの形
	OBB* obb = new OBB();
	obb->Initialize(GetWorldPosition(), worldTransform_.rotation_, worldTransform_.scale_);
	colliderShape_ = obb;


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
	case PlayerBullet::Ice:
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

	// コライダー更新
	colliderShape_->Update(GetWorldPosition(), worldTransform_.rotation_, colliderShape_->GetSize());

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) { 
	switch (bulletType_) {
	case PlayerBullet::None:
		models_[0]->Draw(worldTransform_, viewProjection);
		break;
	case PlayerBullet::Fire:
		models_[1]->Draw(worldTransform_, viewProjection);
		break;
	case PlayerBullet::Ice:
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
	} 
	else {
		// 爆破演出をイージングで行う
		if (animT_ <= explosiveTime_) {
			worldTransform_.scale_ = 
				MyMath::EaseOut(animT_, {0.5f, 0.5f, 0.5f}, explosiveRange_, explosiveTime_);
			// 演出用tを加算
			animT_ += 1.0f / 60.0f;
		} else {
			// 弾を消去する
			isDead_ = true;
		}
	}
}

void PlayerBullet::WaterBulletUpdate() {
	// 床と衝突していなければ
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
		switch (actionWayPoint_) {
		case PlayerBullet::WayPoint1:
			// 展開演出をイージングで行う
			if (animT_ <= deployWallStagingTime_) {
				worldTransform_.scale_ = MyMath::EaseOut(
				    animT_, {1.0f, 0.1f, 1.0f}, {deployWallSize_.x, 0.1f, deployWallSize_.z},
				    deployWallStagingTime_);
				// 演出用tを加算
				animT_ += 1.0f / 60.0f;
			} else {
				// 次の演出時間を設定
				deployWallStagingTime_ = 0.25f;
				// 演出tをリセット
				animT_ = 0.0f;
				// 次の演出へ
				actionWayPoint_++;
			}
			break;
		case PlayerBullet::WayPoint2:
			// 展開演出をイージングで行う
			if (animT_ <= deployWallStagingTime_) {
				worldTransform_.scale_ = MyMath::EaseOut(
				    animT_, {deployWallSize_.x, 0.1f, deployWallSize_.z}, deployWallSize_,
				    deployWallStagingTime_);
				// 演出用tを加算
				animT_ += 1.0f / 60.0f;
			} else {
				// 演出tをリセット
				animT_ = 0.0f;
				// 次の演出へ
				actionWayPoint_++;
			}
			break;
		case PlayerBullet::WayPoint3:
			// 指定時間まで展開
			if (animT_ <= deploymentWallTime_) {
				// 演出用tを加算
				animT_ += 1.0f / 60.0f;
			} else {
				// 演出tをリセット
				animT_ = 0.0f;
				// 次の演出へ
				actionWayPoint_++;
			}
			break;
		case PlayerBullet::WayPoint4:
			// 展開演出をイージングで行う
			if (animT_ <= deployWallEndStagingTime_) {
				worldTransform_.scale_ =
				    MyMath::EaseOut(animT_, deployWallSize_, {deployWallSize_.x, 0.0f, deployWallSize_.z},
				    deployWallStagingTime_);
				// 演出用tを加算
				animT_ += 1.0f / 60.0f;
			} else {
				// 演出tをリセット
				animT_ = 0.0f;
				// 演出終了
				actionWayPoint_ = WayPoint1;
				// 弾を消去
				isDead_ = true;
			}
			break;
		default:
			break;
		}
	}
}

void PlayerBullet::ThunderBulletUpdate() {
	// 何かと衝突していなければ
	if (!isHit_) {
		// 弾をベクトルの方向に前進させる
		worldTransform_.translation_ = worldTransform_.translation_ + velocity_;

		// １フレーム落下した後の座標を一時的に計算
		float tempTranslateY = worldTransform_.translation_.y + fallSpeed_;
		// 一時座標が床以下になったら
		if (tempTranslateY <= 0.0f) {
			// 座標を床位置にリセット
			worldTransform_.translation_.y = 1.0f;
			// 落下速度初期化
			fallSpeed_ = 0.0f;
			// 床と衝突
			isHit_ = true;
		} 
		else {
			// 落下スピード加算
			velocity_.y += fallSpeed_;
			if (fallSpeed_ <= kMaxFallSpeed_) {
				fallSpeed_ -= kFallAcceleration_;
			} else {
				fallSpeed_ = kMaxFallSpeed_;
			}
		}
	} else {
		switch (actionWayPoint_) {
		case PlayerBullet::WayPoint1:
			// 展開演出をイージングで行う
			if (animT_ <= deployAreaStagingTime_) {
				worldTransform_.scale_ = MyMath::EaseIn(
				    animT_, {3.0f, 3.0f, 3.0f}, {2.0f, 2.0f, 2.0f},
				    deployAreaStagingTime_);
				// 演出用tを加算
				animT_ += 1.0f / 60.0f;
			} else {
				deployAreaStagingTime_ = 0.35f;
				// 演出tをリセット
				animT_ = 0.0f;
				// 次の演出へ
				actionWayPoint_++;
			}
			break;
		case PlayerBullet::WayPoint2:
			// 展開演出をイージングで行う
			if (animT_ <= deployAreaStagingTime_) {
				worldTransform_.scale_ = MyMath::EaseOut(
				    animT_, {2.0f, 2.0f, 2.0f}, deployAreaSize_, deployAreaStagingTime_);
				// 演出用tを加算
				animT_ += 1.0f / 60.0f;
			} else {
				// 演出tをリセット
				animT_ = 0.0f;
				// 次の演出へ
				actionWayPoint_++;
			}
			break;
		case PlayerBullet::WayPoint3:
			// 展開
			if (animT_ <= deploymentAreaTime_) {
				// モデルを徐々に回転させる
				worldTransform_.rotation_.y += 0.01f;
				// 振動させる
				float RandomRadius =
				    deployAreaSize_.x + MyMath::RandomF(-shakeRange_, shakeRange_, 2);
				// サイズ変更
				worldTransform_.scale_ = {RandomRadius, 1.0f, RandomRadius};
				// 演出用tを加算
				animT_ += 1.0f / 60.0f;
			} else {
				// 演出tをリセット
				animT_ = 0.0f;
				// 次の演出へ
				actionWayPoint_++;
			}
			break;
		case PlayerBullet::WayPoint4:
			// 展開演出をイージングで行う
			// 展開演出をイージングで行う
			if (animT_ <= deployAreaEndStagingTime_) {
				worldTransform_.scale_ = MyMath::EaseOut(
				    animT_, deployAreaSize_,
				    {deployAreaSize_.x + 10.0f, deployAreaSize_.y, deployAreaSize_.z + 10.0f},
				    deployAreaEndStagingTime_);
				// 演出用tを加算
				animT_ += 1.0f / 60.0f;
			} else {
				// 次の演出時間を設定
				deployAreaEndStagingTime_ = 0.35f;
				// 演出tをリセット
				animT_ = 0.0f;
				// 次の演出へ
				actionWayPoint_++;
			}
			break;
		case PlayerBullet::WayPoint5:
			// 展開演出をイージングで行う
			if (animT_ <= deployAreaEndStagingTime_) {
				worldTransform_.scale_ = MyMath::EaseIn(
				    animT_,
				    {deployAreaSize_.x + 10.0f, deployAreaSize_.y, deployAreaSize_.z + 10.0f},
				    {0.0f, 0.0f, 0.0f}, deployAreaEndStagingTime_);
				// 演出用tを加算
				animT_ += 1.0f / 60.0f;
			} else {
				// 演出tをリセット
				animT_ = 0.0f;
				// 演出終了
				actionWayPoint_ = WayPoint1;
				// 弾を消去
				isDead_ = true;
			}
			break;
		}
	}
}

void PlayerBullet::OnCollision() {

}
