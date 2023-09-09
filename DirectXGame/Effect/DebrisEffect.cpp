#include "DebrisEffect.h"

void DebrisEffect::initialize(
    std::vector<Model*> models, Vector3 translate, const Vector3* occurrenceTranslate,
    const Vector3* occurrenceRotation, float time, Vector2 occurrenceRange) {
	// 基底クラスの初期化
	BaseEffect::initialize(models, translate);
	// 発生座標をポインタで受け取る
	occurrenceTranslate_ = occurrenceTranslate;
	// 発生角度を設定
	occurrenceRotation_ = occurrenceRotation;
	// 継続秒数
	time_ = time;
	// 発生範囲を設定
	occurrenceRange_ = occurrenceRange;

	for (int i = 0; i < kMaxParticleCount_; i++) {
		// 粒子ごとのワールド座標
		worldTransformParticle_[i].Initialize();
		// 親を指定
		worldTransform_.parent_ = (WorldTransform*)&worldTransformParticle_[i];
		// それを発生座標に移動
		worldTransformParticle_[i].translation_ = worldTransform_.translation_;
		// とりあえず終了している状態に
		isEndParticle_[i] = true;
		// 粒子演出用tをリセット
		particleT_[i] = 0.0f;
		// 粒子ごとの最終座標をリセット
		endTransformParticle_[i] = {0.0f, 0.0f, 0.0f};
		// 粒子ごとの行動中間地点リセット
		actionWayPointParticle_[i] = WayPoint1;
	}

	// 発生間隔
	occurrenceInterval_ = 0.0f;
}

void DebrisEffect::Update() {
	// 発生座標のポインタが消失したらエフェクトを強制終了
	if (occurrenceTranslate_ != nullptr) {
		// 継続時間中のエフェクト生成処理
		if (t_ <= time_) {
			if (occurrenceInterval_ <= 0) {
				// 継続時間が終了していない場合は生成間隔ごとに粒子を追加で生成
				for (int i = 0; i < kMaxParticleCount_; i++) {
					// 終了している親パーティクルを見つけたら
					if (isEndParticle_[i]) {

						// 取得した角度から回転行列を生成
						Matrix4x4 rotateMat =
						    MyMath::MakeRotateYMatrix((float)occurrenceRotation_->y);

						// 発生座標を取得
						worldTransform_.translation_.y = occurrenceTranslate_->y;
						// 終了していない粒子の座標を初期化
						worldTransformParticle_[i].translation_ = {0.0f, 0.0f, 0.0f};

						Vector3 offset = {
						    MyMath::RandomF(-occurrenceRange_.x, occurrenceRange_.x, 2),
						    0.0f,
						    MyMath::RandomF(-occurrenceRange_.y, occurrenceRange_.y, 2)};

						Vector3 offset2 = offset;

						// オフセットをカメラの回転に合わせて回転させる
						offset = MyMath::Transform(offset, rotateMat);
						// 初期1を移動
						worldTransformParticle_[i].translation_ =
						    worldTransform_.translation_ + offset;

						// 最終座標をランダムに決める
						offset2.y = MyMath::RandomF(-5.0f, -7.0f, 2);
						// オフセットをカメラの回転に合わせて回転させる
						offset2 = MyMath::Transform(offset2, rotateMat);
						endTransformParticle_[i] = {0.0f, 0.0f, 0.0f};
						endTransformParticle_[i] = worldTransform_.translation_ + offset2;
						// 最終サイズを設定する
						endSizeParticle_[i] = {
						    MyMath::Linear(t_, 0.75f, 0.5f, time_),
						    MyMath::Linear(t_, 0.75f, 0.5f, time_),
						    MyMath::Linear(t_, 0.75f, 0.5f, time_)};

						// 粒子演出用t
						particleT_[i] = 0.0f;
						// 粒子ごとの演出時間
						particleTime_[i] = 0.1f;
						// 演出中間地点リセット
						actionWayPointParticle_[i] = WayPoint1;

						// 親粒子を表示
						isEndParticle_[i] = false;

						// 生成間隔を現在の秒数によって調整
						occurrenceInterval_ = MyMath::EaseIn(t_, 0.0f, 0.2f, time_);

						// 生成出来たらループを抜ける
						break;
					}
				}
			} else {
				// 生成間隔デクリメント
				occurrenceInterval_ -= 1.0f / 60.0f;
			}

			// t加算
			t_ += 1.0f / 60.0f;

		} else {
			// 全粒子が終了状態にあるならエフェクトを終了
			for (int i = 0; i < kMaxParticleCount_; i++) {

				// 粒子が1つでも終了していなければ処理を抜ける
				if (!isEndParticle_[i])
					break;
				// 粒子がすべて終了していればエフェクト終了
				if (i == kMaxParticleCount_ - 1)
					isEnd_ = true;
			}
		}
	} else {
		// 強制終了
		isEnd_ = true;
	}

	// 全体の粒子の更新
	for (int i = 0; i < kMaxParticleCount_; i++) {
		// パーティクルが終了していない時
		if (!isEndParticle_[i]) {
			switch (actionWayPointParticle_[i]) {
			case DebrisEffect::WayPoint1:
				if (particleT_[i] <= particleTime_[i]) {
					// 粒子サイズをイージングで調整
					worldTransformParticle_[i].scale_ = MyMath::EaseIn(
					    particleT_[i], {0.0f, 0.0f, 0.0f}, endSizeParticle_[i], particleTime_[i]);

					// パーティクルの演出tを加算
					particleT_[i] += 1.0f / 60.0f;
				} else {
					// 粒子演出用tをリセット
					particleT_[i] = 0.0f;
					// 粒子演出時間を再設定
					particleTime_[i] = 0.35f;
					// 次の演出へ
					actionWayPointParticle_[i]++;
				}
				break;
			case DebrisEffect::WayPoint2:
				if (particleT_[i] <= particleTime_[i]) {
					// 粒子サイズをイージングで調整
					worldTransformParticle_[i].scale_ = MyMath::EaseIn(
					    particleT_[i], endSizeParticle_[i], {0.0f, 0.0f, 0.0f}, particleTime_[i]);
					// 粒子座標をイージングで調整
					worldTransformParticle_[i].translation_ = MyMath::EaseIn(
					    particleT_[i], worldTransformParticle_[i].translation_,
					    endTransformParticle_[i], particleTime_[i]);

					// パーティクルの演出tを加算
					particleT_[i] += 1.0f / 60.0f;
				} else {
					// 一応スケールをリセット
					worldTransformParticle_[i].scale_ = {0.0f, 0.0f, 0.0f};
					// 粒子を非表示
					isEndParticle_[i] = true;
				}
				break;
			case DebrisEffect::WayPoint3:

				break;
			}
		}

		// ワールド座標の転送
		worldTransformParticle_[i].UpdateMatrix();
	}

	// 基底クラス更新
	BaseEffect::Update();

}

void DebrisEffect::Draw(const ViewProjection& viewProjection) {
	// モデル描画
	for (int i = 0; i < kMaxParticleCount_; i++) {
		// パーティクルが終了していない時
		if (!isEndParticle_[i]) {
			models_[0]->Draw(worldTransformParticle_[i], viewProjection);
		}
	}
}
