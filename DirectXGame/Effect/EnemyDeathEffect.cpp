#include "EnemyDeathEffect.h"
#include <numbers>
#include "MyMath.h"

void EnemyDeathEffect::initialize(
    std::vector<Model*> models, uint32_t textureHandle, Vector3 translate) {
	// 基底クラスの初期化
	BaseEffect::initialize(models, translate);

	Vector3 rotate = {0.0f, 0.0f, 0.0f};
	// 粒子ごとの初期化
	for (int i = 0; i < kMaxParticleCount_; i++) {
		// 粒子ごとのワールド座標初期化
		worldTransformParticle_[i].Initialize();
		// 粒子座標を発生地点にリセット
		worldTransformParticle_[i].translation_ = translate;
	
		// 粒子の速度
		float speed = 5.0f;
		if (i / 8 == 0) {
			rotate.x = 0.0f;
			rotate.y += float(std::numbers::pi) / 4.0f;
		} else if (i / 8 == 1) {
			rotate.x += float(std::numbers::pi) / 4.0f;
			rotate.y = 0.0f;
		} else if (i / 8 == 2) {
			rotate.x = float(std::numbers::pi) / 4.0f;
			rotate.y += float(std::numbers::pi) / 4.0f;
		} else {
			rotate.x = float(std::numbers::pi) / -4.0f;
			rotate.y += float(std::numbers::pi) / 4.0f;
		}
		velocityParticle_[i].z +=
		    std::cosf(rotate.y) * std::cosf(rotate.x) * speed; // コサイン
		velocityParticle_[i].x +=
		    std::sinf(rotate.y) * std::cosf(rotate.x) * speed;
		velocityParticle_[i].y +=
		    -std::sinf(rotate.x) * std::cosf(rotate.z) * speed;
	
	}

	// タイマー
	timer_ = time_;
	// テクスチャハンドル
	textureHandle_ = textureHandle;

}

void EnemyDeathEffect::Update() {

	//サイズ変更準備
	float t = float(timer_) / float(time_);

	// 粒子ごとに更新
	for (int i = 0; i < kMaxParticleCount_; i++) {
		// 粒子を移動
		worldTransformParticle_[i].translation_ =
		    worldTransformParticle_[i].translation_ + velocityParticle_[i];
	
		worldTransformParticle_[i].scale_.x = MyMath::Linear(t, 0.0f, 1.0f);
		worldTransformParticle_[i].scale_.y = MyMath::Linear(t, 0.0f, 1.0f);
		worldTransformParticle_[i].scale_.z = MyMath::Linear(t, 0.0f, 1.0f);

		// ワールド行列の更新
		worldTransformParticle_[i].UpdateMatrix();

	}

	// タイマー処理
	if (--timer_ == 0) {
		isEnd_ = true;
	}

	// 基底クラスの更新
	BaseEffect::Update(); 

}

void EnemyDeathEffect::Draw(const ViewProjection& viewProjection) {

	// 粒子ごとに描画
	for (int i = 0; i < kMaxParticleCount_; i++) {
		models_[0]->Draw(worldTransformParticle_[i], viewProjection,textureHandle_);
	}
}
