#include "ExplosiveEffect.h"

void ExplosiveEffect::initialize(std::vector<Model*> models, Vector3 translate, float strength) {
	// 基底クラスの初期化
	BaseEffect::initialize(models, translate);

	// 粒子ごとの初期化
	for (int i = 0; i < kMaxParticleCount_; i++) {
		// 粒子ごとのワールド座標初期化
		worldTransformParticle_[i].Initialize();
		// 粒子座標を発生地点にリセット
		worldTransformParticle_[i].translation_ = translate;
		// 粒子のサイズを設定
		float scale = MyMath::RandomF(strength / 7.0f, strength / 5.0f, 2);
		worldTransformParticle_[i].scale_ = {
		    scale, scale, scale};
		// 粒子ごとの終了状態をリセット
		isEndParticle_[i] = false;
		// 粒子ごとの速度を設定
		velocityParticle_[i] = {
		    MyMath::RandomF(-1.0f, 1.0f, 2), MyMath::RandomF(0.5f, 1.0f, 2),
		    MyMath::RandomF(-1.0f, 1.0f, 2)};
		velocityParticle_[i] = velocityParticle_[i] * strength;
		// 落下速度リセット
		fallSpeed_[i] = 0.0f;
	}

	// 粒子の落下加速度設定
	kFallAcceleration_ = 0.0098f;
}

void ExplosiveEffect::Update() { 
	// 全粒子が終了状態にあるならエフェクトを終了
	for (int i = 0; i < kMaxParticleCount_; i++) {

		// 粒子が1つでも終了していなければ処理を抜ける
		if (!isEndParticle_[i])
			break;
		// 粒子がすべて終了していればエフェクト終了
		if (i == kMaxParticleCount_ - 1)
			isEnd_ = true;
	}

	// 粒子ごとに更新
	for (int i = 0; i < kMaxParticleCount_; i++) {
		// 粒子が終了状態でない時に
		if (!isEndParticle_[i]) {
			// 粒子を移動
			worldTransformParticle_[i].translation_ =
			    worldTransformParticle_[i].translation_ + velocityParticle_[i];

			worldTransformParticle_[i].rotation_.x += 0.1f;
			// １フレーム落下した後の座標を一時的に計算
			float tempTranslateY = worldTransformParticle_[i].translation_.y + fallSpeed_[i];
			// 一時座標が床以下になったら
			if (tempTranslateY <= -1.0f) {
				// 粒子終了状態に
				isEndParticle_[i] = true;
			} else {
				// 落下スピード加算
				velocityParticle_[i].y += fallSpeed_[i];
				if (fallSpeed_[i] <= 5.0f) {
					fallSpeed_[i] -= kFallAcceleration_;
				} else {
					fallSpeed_[i] = 5.0f;
				}
			}
			// ワールド行列の更新
			worldTransformParticle_[i].UpdateMatrix();
		}
	}

	// 基底クラスの更新
	BaseEffect::Update(); 
}

void ExplosiveEffect::Draw(const ViewProjection& viewProjection) { 
	// 粒子ごとに描画
	for (int i = 0; i < kMaxParticleCount_; i++) {
		if (!isEndParticle_[i]) {
			models_[0]->Draw(worldTransformParticle_[i], viewProjection);
		}
	}
}
