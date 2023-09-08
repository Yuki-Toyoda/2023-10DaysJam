#include "SparkEffect.h"


void SparkEffect::initialize(
    std::vector<Model*> models, Vector3 translate, float time, Vector2 occurrenceRange,
    const ViewProjection* viewProjection) {
	// 基底クラスの初期化
    BaseEffect::initialize(models, translate);
	// 継続秒数
	time_ = time;
	// 発生範囲を設定
	occurrenceRange_ = occurrenceRange;
	// ビュープロジェクションセット
	viewProjection_ = viewProjection;

	for (int i = 0; i < kMaxParticleCount_; i++) {
		// 粒子ごとのワールド座標
		worldTransformParticle_[i].Initialize();
		// それを発生座標に移動
		worldTransformParticle_[i].translation_ = worldTransform_.translation_;
		// とりあえず終了している状態に
		isEndParticle_[i] = true;
		// 粒子演出用tをリセット
		particleT_[i] = 0.0f;
		// 粒子ごとの最終座標をリセット
		endTransformParticle_[i] = {0.0f, 0.0f, 0.0f};
	}

	// 発生間隔
	occurrenceInterval_ = 0.0f;

}

void SparkEffect::Update() {
	// 継続時間中のエフェクト生成処理
	if (t_ <= time_) {
		if (occurrenceInterval_ <= 0) {
			// 継続時間が終了していない場合は生成間隔ごとに粒子を追加で生成
			for (int i = 0; i < kMaxParticleCount_; i++) {
				// 終了している親パーティクルを見つけたら
				if (isEndParticle_[i]) {
					// 生存していない粒子の座標を初期化
					worldTransformParticle_[i].translation_ = {
					    MyMath::RandomF(
					        worldTransform_.translation_.x - occurrenceRange_.x,
					        worldTransform_.translation_.x + occurrenceRange_.x, 2),
					    worldTransform_.translation_.y,
					    MyMath::RandomF(
					        worldTransform_.translation_.z - occurrenceRange_.y,
					        worldTransform_.translation_.z + occurrenceRange_.y, 2)};
					// 最終座標をランダムに決める
					Vector3 Velocity = {
						MyMath::RandomF(-0.25f, 0.25f, 2),
						MyMath::RandomF(0.75f, 1.0f, 2),
					    MyMath::RandomF(-0.25f, 0.25f, 2)};
					endTransformParticle_[i] = worldTransformParticle_[i].translation_;
					endTransformParticle_[i] =
					    endTransformParticle_[i] + (Velocity * MyMath::RandomF(10.0f, 17.0f, 2));

					// 粒子演出用t
					particleT_[i] = 0.0f;
					// 粒子ごとの演出時間
					particleTime_[i] = 1.0f;

					// 親粒子を表示
					isEndParticle_[i] = false;

					// 生成間隔を現在の秒数によって調整
					occurrenceInterval_ = MyMath::EaseIn(t_, 0.1f, 3.0f, time_);

					// 生成出来たらループを抜ける
					break;
				}
			}
		}
		else {
			// 生成間隔デクリメント
			occurrenceInterval_ -= 1.0f / 60.0f;
		}

		// t加算
		t_ += 1.0f / 60.0f;

	} 
	else {

		// 全粒子が終了状態にあるならエフェクトを終了
		for (int i = 0; i < kMaxParticleCount_ ; i++) {

			// 粒子が1つでも終了していなければ処理を抜ける
			if (!isEndParticle_[i])
					break;
			// 粒子がすべて終了していればエフェクト終了
			if (i == kMaxParticleCount_ - 1)
					isEnd_ = true;
		}
	}

	// 全体の粒子の更新
	for (int i = 0; i < kMaxParticleCount_; i++) {
		// パーティクルが終了していない時
		if (!isEndParticle_[i]) {
			if (particleT_[i] <= particleTime_[i]) {
				    
				worldTransformParticle_[i].translation_ = MyMath::EaseIn(
				        particleT_[i], worldTransformParticle_[i].translation_,
				        endTransformParticle_[i],
				        particleTime_[i]);

				worldTransformParticle_[i].scale_ = MyMath::EaseIn(
					particleT_[i], 
					{0.0f, 0.0f, 0.0f}, 
					{1.0f, 1.0f, 1.0f}, 
					particleTime_[i]);

				// パーティクルの演出tを加算
				particleT_[i] += 1.0f / 60.0f;
			} else {
				// 粒子の終了
				isEndParticle_[i] = true;
			}
		}
		// ビルボード処理
		// ビュー行列を取得
		Matrix4x4 viewMatrix = MyMath::Inverse(viewProjection_->matView);
		// ビュー行列の移動要素を消す
		viewMatrix.m[3][0] = 0.0f;
		viewMatrix.m[3][1] = 0.0f;
		viewMatrix.m[3][2] = 0.0f;

		// ワールド行列の計算
		Matrix4x4 worldMatrix = MyMath::MakeAffineMatrix(
		    worldTransformParticle_[i].scale_, worldTransformParticle_[i].rotation_,
		    worldTransformParticle_[i].translation_);
		worldTransformParticle_[i].matWorld_ = viewMatrix * worldMatrix;

		// ワールド座標の転送
		worldTransformParticle_[i].TransferMatrix();
	}

	// 基底クラスの初期化
	BaseEffect::Update();
}

void SparkEffect::Draw(const ViewProjection& viewProjection) { 
	// モデル描画
	models_[0]->Draw(worldTransformParticle_[0], viewProjection);
	for (int i = 0; i < kMaxParticleCount_; i++) {
		// パーティクルが終了していない時
		if (!isEndParticle_[i]) {
			models_[0]->Draw(worldTransformParticle_[i], viewProjection);
		}
	}
}
