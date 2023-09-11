#include "PlayerBullet.h"
#include "../Effect/EffectManager.h"
#include "Collision/ColliderShape/OBB.h"

void PlayerBullet::Initialize(
    const std::vector<Model*>& models,
    Vector3 startPos, Vector3 startotation, Vector3 velocity) {
	// 基底クラス初期化
	BaseCharacter::Initialize(models);

	// モデル受け取り
	models_ = models;

	// ワールド座標初期化
	worldTransform_.Initialize();

	// 音再生インスタンス取得
	audio_ = Audio::GetInstance();

	// 引数の値をメンバ変数に代入
	worldTransform_.translation_ = startPos;
	worldTransform_.rotation_ = startotation;
	velocity_ = velocity;
	bulletType_ = None;

	// 生存している状態に
	isDead_ = false;
	// 生存時間設定
	deathTimer_ = klifeTime_;
	
	// 反射係数
	reflectionCoefficient = 0.8f;

	// 衝突属性を設定
	SetCollisionAttribute(0xfffffffe);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(0x00000001);
	// タグ
	tag_ = TagPlayerBulletNone;

	// コライダーの形
	OBB* obb = new OBB();
	obb->Initialize(GetWorldPosition(), worldTransform_.rotation_, worldTransform_.scale_);
	colliderShape_ = obb;

}

void PlayerBullet::Initialize(
    const std::vector<Model*>& models, const std::vector<uint32_t>& textureHandles,
    const std::vector<uint32_t>& audioHandles, Vector3 startPos, Vector3 startotation,
    Vector3 velocity, BulletType bulletType, int32_t bulletStrength) {
	// 基底クラス初期化
	BaseCharacter::Initialize(models);

	// モデル受け取り
	models_ = models;
	// テクスチャ受け取り
	textureHandles_ = textureHandles;
	// 効果音受け取り
	audioHandles_ = audioHandles;

	// ワールド座標初期化
	worldTransform_.Initialize();

	// 音再生インスタンス取得
	audio_ = Audio::GetInstance();

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
		// 大きさを設定
		worldTransform_.scale_ = {0.01f, 0.01f, 0.01f};
		// 爆破範囲設定
		explosiveRange_ = {
		    1.0f * (1.0f + 0.45f * (bulletStrength - 1)),
		    1.0f * (1.0f + 0.45f * (bulletStrength - 1)),
		    1.0f * (1.0f + 0.45f * (bulletStrength - 1))};
		// 爆破演出時間
		explosiveTime_ = 0.25f;
		// タグ
		tag_ = TagPlayerBulletFire;

		break;
	case PlayerBullet::Ice:
		// テクスチャ用行動中間地点
		actionWayPointTexture_ = WayPoint1;
		// 描画テクスチャ番号指定
		drawTextureNumber_ = 0;
		// 大きさを設定
		worldTransform_.scale_ = {0.01f, 0.01f, 0.01f};
		// 回転角をリセット
		worldTransform_.rotation_ = {0.0f, worldTransform_.rotation_.y, 0.0f};
		// 壁のサイズ設定
		deployWallSize_ = {
		    1.0f * (1.0f + 0.35f * (bulletStrength - 1)),
		    1.0f * (1.0f + 0.15f * (bulletStrength - 1)),
		    1.0f * (1.0f + 0.15f * (bulletStrength - 1))};
		deployWallSizeParticle_ = {
		    50.0f * (1.0f + 0.35f * (bulletStrength - 1)),
		    20.0f * (1.0f + 0.15f * (bulletStrength - 1)),
		    20.0f * (1.0f + 0.15f * (bulletStrength - 1))};
		// 展開演出時間設定
		deployWallStagingTime_ = 0.35f;
		// 展開時間設定
		deploymentWallTime_ = 5.0f * (1.0f + 0.35f * (bulletStrength - 1)),
		// 終了演出時間設定
		    deployWallEndStagingTime_ = 0.1f;
		// タグ
		tag_ = TagPlayerBulletIce;
		break;
	case PlayerBullet::Thunder:
		// 大きさを設定
		worldTransform_.scale_ = {0.01f, 0.01f, 0.01f};
		// 回転角をリセット
		worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
		// 展開エリアを設定
		deployAreaSize_ = {
		    1.0f * (1.0f + 0.35f * (bulletStrength - 1)),
		    1.0f,
		    1.0f * (1.0f + 0.35f * (bulletStrength - 1)),
		};
		// 振動範囲を設定
		shakeRange_ = 0.05f * (1.0f + 0.35f * (bulletStrength - 1));
		// 展開演出時間
		deployAreaStagingTime_ = 0.25f;
		// 展開時間
		deploymentAreaTime_ = 7.0f * (1.0f + 0.35f * (bulletStrength - 1));
		// 終了演出時間
		deployAreaEndStagingTime_ = 0.15f;
		// タグ
		tag_ = TagPlayerBulletThunder;

		break;
	}

	// 生存している状態に
	isDead_ = false;
	// 生存時間設定
	deathTimer_ = klifeTime_;

	// 反射係数
	reflectionCoefficient = 0.8f;

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
		IceBulletUpdate();
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
	ColliderUpdate();

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
		models_[2]->Draw(worldTransform_, viewProjection, textureHandles_[drawTextureNumber_]);
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

		std::vector<Model*> EffectModels = {models_[4]}; // エフェクト用モデルリストの生成

		switch (actionWayPoint_) {
		case PlayerBullet::WayPoint1:
			
			// 爆発音を再生
			audio_->PlayWave(audioHandles_[Audio::FireBullet]);

			// 破片エフェクト再生を指示
			EffectManager::GetInstance()->PlayExplosiveEffect(
			    EffectModels,
			    {
			        worldTransform_.translation_.x,
			        worldTransform_.translation_.y + 2.0f,
			        worldTransform_.translation_.z,
			    },
			    5.0f * (1.0f + 0.5f * (bulletStrength_ - 1)));
			actionWayPoint_++;
			break;
		case PlayerBullet::WayPoint2:
			// 爆破演出をイージングで行う
			if (animT_ <= explosiveTime_) {
				worldTransform_.scale_ =
				    MyMath::EaseOut(animT_, {0.01f, 0.01f, 0.01f}, explosiveRange_, explosiveTime_);
				// 演出用tを加算
				animT_ += 1.0f / 60.0f;
			} else {
				// 弾を消去する
				isDead_ = true;
			}
			break;
		}
	}
}

void PlayerBullet::IceBulletUpdate() {
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
			// 衝突属性を設定
			SetCollisionAttribute(0xfffffff7);
			// 衝突対象を自分の属性以外に設定
			SetCollisionMask(0x00000008);

			// 展開音を再生
			audio_->PlayWave(audioHandles_[Audio::IceBulletDeploy]);

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
				    animT_, {0.01f, 0.01f, 0.01f}, {deployWallSize_.x, 0.01f, deployWallSize_.z},
				    deployWallStagingTime_);
				// 演出用tを加算
				animT_ += 1.0f / 60.0f;
			} else {

				// 次の演出時間を設定
				deployWallStagingTime_ = 1.0f;

				deployWallSizeParticle_ = {
				    50.0f * (1.0f + 0.35f * (bulletStrength_ - 1)),
				    0.01f,
				    20.0f * (1.0f + 0.15f * (bulletStrength_ - 1))};

				std::vector<Model*> EffectModels = {models_[5]}; // エフェクト用モデルリストの生成
				// 破片エフェクト再生を指示
				EffectManager::GetInstance()->PlayDebrisEffect(
				    EffectModels,
				    {
				        worldTransform_.translation_.x,
				        worldTransform_.translation_.y + 2.0f,
				        worldTransform_.translation_.z,
				    },
				    (Vector3*)&deployWallSizeParticle_, 
					(Vector3*)&worldTransform_.rotation_,
					deployWallStagingTime_ - 0.1f,
				    {50.0f * (1.0f + 0.35f * (bulletStrength_ - 1)),
				     20.0f * (1.0f + 0.15f * (bulletStrength_ - 1))});

				// 演出tをリセット
				animT_ = 0.0f;
				// 次の演出へ
				actionWayPoint_++;
			}
			break;
		case PlayerBullet::WayPoint2:
			// 展開演出をイージングで行う
			if (animT_ <= deployWallStagingTime_) {
				// 壁の大きさ設定
				worldTransform_.scale_ = MyMath::EaseOut(
				    animT_, {deployWallSize_.x, 0.01f, deployWallSize_.z}, deployWallSize_,
				    deployWallStagingTime_);
				deployWallSizeParticle_ = MyMath::EaseOut(
				    animT_,
				    {50.0f * (1.0f + 0.35f * (bulletStrength_ - 1)),
				     0.2f,
				     20.0f * (1.0f + 0.15f * (bulletStrength_ - 1))},
				    {50.0f * (1.0f + 0.35f * (bulletStrength_ - 1)),
				     40.0f * (1.0f + 0.15f * (bulletStrength_ - 1)),
				     20.0f * (1.0f + 0.15f * (bulletStrength_ - 1))},
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
				switch (actionWayPointTexture_) {
				case PlayerBullet::WayPoint1:
					// 指定秒数以上になったらテクスチャ変更とエフェクト再生
					if (animT_ >= deploymentWallTime_ / 3.0f) {
						// テクスチャ変更
						drawTextureNumber_ = 1;
						// エフェクト再生
						std::vector<Model*> EffectModels = {
						    models_[5]}; // エフェクト用モデルリストの生成
						                 // 破片エフェクト再生を指示
						EffectManager::GetInstance()->PlayExplosiveEffect(
						    EffectModels,
						    {
						        worldTransform_.translation_.x,
						        worldTransform_.translation_.y + (deployWallSizeParticle_.y / 2.0f),
						        worldTransform_.translation_.z,
						    },
						    2.0f);

						// ダメージ音を再生
						audio_->PlayWave(audioHandles_[Audio::IceBulletDamage]);

						// 次の演出へ
						actionWayPointTexture_++;
					}
					break;
				case PlayerBullet::WayPoint2:
					// 指定秒数以上になったらテクスチャ変更とエフェクト再生
					if (animT_ >= deploymentWallTime_ / 1.5f) {
						// テクスチャ変更
						drawTextureNumber_ = 2;
						// エフェクト再生
						std::vector<Model*> EffectModels = {
						    models_[5]}; // エフェクト用モデルリストの生成
						                 // 破片エフェクト再生を指示
						EffectManager::GetInstance()->PlayExplosiveEffect(
						    EffectModels,
						    {
						        worldTransform_.translation_.x,
						        worldTransform_.translation_.y + (deployWallSizeParticle_.y / 2.0f),
						        worldTransform_.translation_.z,
						    },
						    2.5f);

						// ダメージ音を再生
						audio_->PlayWave(audioHandles_[Audio::IceBulletDamage]);

						// 次の演出へ
						actionWayPointTexture_++;
					}
					break;
				case PlayerBullet::WayPoint3:
					break;
				}

				// 演出用tを加算
				animT_ += 1.0f / 60.0f;
			} else {

				// エフェクト再生
				std::vector<Model*> EffectModels = {models_[5]}; // エフェクト用モデルリストの生成
				                                                 // 破片エフェクト再生を指示
				EffectManager::GetInstance()->PlayExplosiveEffect(
				    EffectModels,
				    {
				        worldTransform_.translation_.x,
				        worldTransform_.translation_.y + (deployWallSizeParticle_.y / 2.0f),
				        worldTransform_.translation_.z,
				    },
				    3.0f);

				// 破壊音を再生
				audio_->PlayWave(audioHandles_[Audio::IceBulletDestroy]);

				// 演出tをリセット
				animT_ = 0.0f;
				// 次の演出へ
				actionWayPoint_++;
			}
			break;
		case PlayerBullet::WayPoint4:
			// 展開演出をイージングで行う
			if (animT_ <= deployWallEndStagingTime_) {
				worldTransform_.scale_ = MyMath::EaseIn(
				    animT_, deployWallSize_,
				    {deployWallSize_.x + 0.25f, deployWallSize_.y + 0.25f,
				     deployWallSize_.z + 0.25f},
				    deployWallEndStagingTime_);
				// 演出用tを加算
				animT_ += 1.0f / 60.0f;
			} else {
				// スケールリセット
				worldTransform_.scale_ = {0.0f, 0.0f, 0.0f};
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

			// 展開開始音再生
			audio_->PlayWave(audioHandles_[Audio::ThunderBulletStart]);

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
				    animT_, {0.1f, 0.1f, 0.1f}, {0.05f, 0.05f, 0.05f},
				    deployAreaStagingTime_);
				// 演出用tを加算
				animT_ += 1.0f / 60.0f;
			} else {

				// 再生されていなければ展開音を再生する
				if (!audio_->IsPlaying(voiceHandleDeployThunderArea_) ||
				    voiceHandleDeployThunderArea_ == -1) {
					voiceHandleDeployThunderArea_ =
					    audio_->PlayWave(audioHandles_[Audio::ThunderBulletDeploy]);
				}

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
				    animT_, {0.05f, 0.05f, 0.05f}, deployAreaSize_,
				    deployAreaStagingTime_);
				// 演出用tを加算
				animT_ += 1.0f / 60.0f;
			} else {

				std::vector<Model*> EffectModels = {models_[6]}; // エフェクト用モデルリストの生成
				// 破片エフェクト再生を指示
				EffectManager::GetInstance()->PlaySparkEffect(
				    EffectModels,
				    {
				        worldTransform_.translation_.x,
				        worldTransform_.translation_.y + 2.0f,
				        worldTransform_.translation_.z,
				    },
				    deploymentAreaTime_,
				    {70.0f * (1.0f + 0.35f * (bulletStrength_ - 1)),
				     70.0f * (1.0f + 0.35f * (bulletStrength_ - 1))});

				// 再生されていなければ展開音を再生する
				if (!audio_->IsPlaying(voiceHandleDeployThunderArea_) ||
				    voiceHandleDeployThunderArea_ == -1) {
					voiceHandleDeployThunderArea_ =
					    audio_->PlayWave(audioHandles_[Audio::ThunderBulletDeploy]);
				}

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
				worldTransform_.scale_ = {RandomRadius, deployAreaSize_.y, RandomRadius};
				
				// 再生されていなければ展開音を再生する
				if (!audio_->IsPlaying(voiceHandleDeployThunderArea_) ||
				    voiceHandleDeployThunderArea_ == -1) {
					voiceHandleDeployThunderArea_ =
					    audio_->PlayWave(audioHandles_[Audio::ThunderBulletDeploy]);
				}

				// 演出用tを加算
				animT_ += 1.0f / 60.0f;
			} else {

				// 展開音は止める
				Audio::GetInstance()->StopWave(voiceHandleDeployThunderArea_);

				// 展開終了音再生
				audio_->PlayWave(audioHandles_[Audio::ThunderBulletDeployEnd]);

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
				    {deployAreaSize_.x + 0.5f, deployAreaSize_.y, deployAreaSize_.z + 0.5f},
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
				    animT_, {deployAreaSize_.x + 0.5f, deployAreaSize_.y, deployAreaSize_.z + 0.5f},
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

void PlayerBullet::ColliderUpdate() {

	Vector3 pos = GetWorldPosition();
	Vector3 rotate = worldTransform_.rotation_;
	Vector3 scale = worldTransform_.scale_;

	switch (bulletType_) {
	case PlayerBullet::None:
		// 通常弾更新
		scale = scale * 2.0f;
		colliderShape_->Update(pos, rotate, scale);
		break;
	case PlayerBullet::Fire:
		// 炎弾更新
		scale = scale * 50.0f;
		colliderShape_->Update(pos, rotate, scale);
		break;
	case PlayerBullet::Ice:
		// 水弾更新
		pos.y += worldTransform_.scale_.y * 20.0f;
		scale.x *= 50.0f;
		scale.y *= 20.0f;
		scale.z *= 6.0f;
		colliderShape_->Update(pos, rotate, scale);
		break;
	case PlayerBullet::Thunder:
		// 雷弾更新
		scale.x *= 65.0f;
		scale.y *= 200.0f;
		scale.z *= 65.0f;
		colliderShape_->Update(pos, rotate, scale);
		break;
	default:
		break;
	}

}

void PlayerBullet::OnCollision(Collider* collision) {

	switch (bulletType_) {
	case PlayerBullet::None:
		isDead_ = true;
		break;
	case PlayerBullet::Fire:
		if (!isHit_) {
			isHit_ = true;
		}
		break;
	case PlayerBullet::Ice:
		if (!isHit_) {
			if (collision->GetTag() == TagPlayerBulletIce ||
			    collision->GetTag() == TagEnemy ||
			    collision->GetTag() == TagBossEnemy ||
				collision->GetTag() == TagEnemyBullet) {
				velocity_ = velocity_ * -reflectionCoefficient;
			}
		}
		break;
	case PlayerBullet::Thunder:
		if (!isHit_) {
			if (collision->GetTag() == TagPlayerBulletIce || collision->GetTag() == TagEnemy ||
			    collision->GetTag() == TagBossEnemy || collision->GetTag() == TagEnemyBullet) {
				velocity_ = velocity_ * -reflectionCoefficient;
			}
		}
		break;
	default:
		break;
	}
}
