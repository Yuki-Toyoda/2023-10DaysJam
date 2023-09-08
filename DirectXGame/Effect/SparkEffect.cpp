#include "SparkEffect.h"

void SparkEffect::initialize(
    std::vector<Model*> models, Vector3 translate, float time, Vector2 occurrenceRange) {
	// 基底クラス初期化
	BaseEffect::initialize(models, translate);
	// 継続時間設定
	time_ = time;
	// 範囲設定
	occurrenceRange_ = occurrenceRange;

	// 全体の粒子の初期化
	for (int i = 0; i < kMaxParticleCount_; i++) {
		// 粒子のワールド座標初期化
		worldTransformParticle_[i].Initialize();
		// 粒子の発生座標
		worldTransformParticle_[i].translation_ = translate;
		// 粒子ごとの発生座標をランダム化
		worldTransformParticle_[i].translation_ = {
		    MyMath::RandomF(
		        worldTransformParticle_[i].translation_.x - occurrenceRange_.x,
		        worldTransformParticle_[i].translation_.x + occurrenceRange_.x, 2),
		    MyMath::RandomF(
		        worldTransformParticle_[i].translation_.y,
		        worldTransformParticle_[i].translation_.y, 2),
		    MyMath::RandomF(
		        worldTransformParticle_[i].translation_.z - occurrenceRange_.y,
		        worldTransformParticle_[i].translation_.z + occurrenceRange_.y, 2)
		};
		// 一度粒子を終了状態に
		isEndParticle_[i] = true;
		// 粒子ごとの速度設定
		velocityParticle_[i] = {0.0f, 0.0f, 0.0f};
		// 粒子ごとの継続時間リセット
		particleActiveTime_[i] = 1.0f;
		// 粒子ごとの落下速度を設定
		fallSpeed_[i] = 0.0f;
		// 粒子ごとのtをリセット
		particleT_[i] = 0.0f;
		// 粒子の演出段階リセット
		actionWayPoint_[i] = WayPoint1;

		// 子粒子の初期化
		for (int j = (i * kMaxChildParticleCount_ - 1); j < kMaxChildParticleCount_ * (i + 1);
		     j++) {
			// jが0以下なら
			if (j < 0) {
				// jを0に
				j = 0;
			}
			// 子粒子のワールド座標初期化
			worldTransformChildParticle_[j].Initialize();
			// 子粒子の座標をそれぞれの粒子に移動
			worldTransformChildParticle_[j].translation_ = worldTransformParticle_[i].translation_;
			// 子粒子も一度終了状態に
			isEndChildParticle_[j] = true;
			// 子粒子の速度をリセット
			velocityChildParticle_[j] = {0.0f, 0.0f, 0.0f};

		}
	}

	// ループを抜けるフラグ
	roopBreak_ = false;

	// 粒子落下加速度
	kFallAcceleration_ = 0.098f;
	// 演出t初期化
	t_ = 0.0f;
	// 粒子発生間隔初期化
	occurrenceInterval_ = 0.0f;

}

void SparkEffect::Update() { 
	// エフェクトの継続時間中なら
	if (t_ < time_) {
		if (occurrenceInterval_ <= 0) {
			// 継続時間が終了していない場合は生成間隔ごとに粒子を追加で生成
			for (int i = 0; i < kMaxParticleCount_; i++) {
				// 終了している親パーティクルを見つけたら
				if (isEndParticle_[i]) {
					// 子粒子の初期化
					for (int j = (i * kMaxChildParticleCount_ - 1);
					     j < kMaxChildParticleCount_ * (i + 1); j++) {
						// jが0以下なら
						if (j < 0) {
							// jを0に
							j = 0;
						}

						// 1つでも終了していない子粒子があれば
						if (!isEndChildParticle_[i]) {
							// ループを抜ける
							break;
						}

						// 最後まで到達したら
						if (j == (kMaxChildParticleCount_ * (i + 1)) - 1) {
							// 生存していない粒子の座標を初期化
							worldTransformParticle_[i].translation_ = {
							    MyMath::RandomF(
							        worldTransform_.translation_.x - occurrenceRange_.x,
							        worldTransform_.translation_.x + occurrenceRange_.x,
							        2),
							    worldTransform_.translation_.y,
							    MyMath::RandomF(
							        worldTransform_.translation_.z - occurrenceRange_.y,
							        worldTransform_.translation_.z + occurrenceRange_.y,
							        2)};
							// 親粒子を表示
							isEndParticle_[i] = false;
							// 粒子ごとの速度設定
							velocityParticle_[i] = {
							    MyMath::RandomF(-0.5f, 0.5f, 2), MyMath::RandomF(0.75f, 1.0f, 2),
							    MyMath::RandomF(-0.5f, 0.5f, 2)};
							// 速度を加速させる
							velocityParticle_[i] = velocityParticle_[i] * 9.0f;
							// 粒子ごとの継続時間リセット
							particleActiveTime_[i] = 0.35f;
							// 粒子ごとの落下速度リセット
							fallSpeed_[i] = 0.0f;
							// 粒子ごとのtをリセット
							particleT_[i] = 0.0f;
							// 粒子の演出段階リセット
							actionWayPoint_[i] = WayPoint1;

							// 子粒子の座標をそれぞれの粒子に移動
							worldTransformChildParticle_[j].translation_ =
							    worldTransformParticle_[i].translation_;
							// 子粒子の速度をリセット
							velocityChildParticle_[j] = {0.0f, 0.0f, 0.0f};

							// ループを終了
							roopBreak_ = true;

							// 生成間隔を現在の秒数によって調整
							occurrenceInterval_ = MyMath::EaseIn(t_, 0.1f, 3.0f, time_);

							// 生成出来たらループを抜ける
							break;

						}
					}
				}
				// ループ抜けフラグがTrueなら
				if (roopBreak_) {
					// ループを抜けるフラグfalse
					roopBreak_ = false;
					// ループを抜ける
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
	} else {
		// 全粒子が終了状態にあるならエフェクトを終了
		for (int i = 0; i < kMaxParticleCount_ * kMaxChildParticleCount_; i++) {

			// 粒子が1つでも終了していなければ処理を抜ける
			if (!isEndChildParticle_[i])
				break;
			// 粒子がすべて終了していればエフェクト終了
			if (i == (kMaxParticleCount_ * kMaxChildParticleCount_) - 1)
				isEnd_ = true;
		}
	}

	// 全体の粒子の更新
	for (int i = 0; i < kMaxParticleCount_; i++) {
		// パーティクルが終了していない時
		if (!isEndParticle_[i]) {
			switch (actionWayPoint_[i]) {
			case SparkEffect::WayPoint1:
				// パーティクルの継続時間の間処理を行う
				if (particleT_[i] <= particleActiveTime_[i]) {
					// 粒子のワールド座標を加算
					worldTransformParticle_[i].translation_ =
					    worldTransformParticle_[i].translation_ + velocityParticle_[i];
					worldTransformParticle_[i].scale_ = MyMath::EaseIn(
					    particleT_[i], {0.15f, 0.15f, 0.15f}, {1.0f, 1.0f, 1.0f},
					    particleActiveTime_[i]);
					// パーティクルのtを加算
					particleT_[i] += 1.0f / 60.0f;
					// 落下スピード加算
					velocityParticle_[i].y += fallSpeed_[i];
					if (fallSpeed_[i] <= 5.0f) {
						fallSpeed_[i] -= kFallAcceleration_;
					} else {
						fallSpeed_[i] = 5.0f;
					}
				} else {
					// 粒子演出時間設定
					particleActiveTime_[i] = 0.25f;
					// 粒子用t初期化
					particleT_[i] = 0.0f;
					// 次の演出段階へ
					actionWayPoint_[i]++;
				}
				break;
			case SparkEffect::WayPoint2:
				// パーティクルの継続時間の間処理を行う
				if (particleT_[i] <= particleActiveTime_[i]) {
					// 大きさを調節
					worldTransformParticle_[i].scale_ = MyMath::EaseIn(
					    particleT_[i], {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f},
					    particleActiveTime_[i]);
					// パーティクルのtを加算
					particleT_[i] += 1.0f / 60.0f;
				} else {
					isEndParticle_[i] = true;
				}
				break;
			}

			// ワールド座標の更新
			worldTransformParticle_[i].UpdateMatrix();
		}

		// 子粒子の初期化
		for (int j = (i * kMaxChildParticleCount_ - 1); j < kMaxChildParticleCount_ * (i + 1);
		     j++) {
			// jが0以下なら
			if (j < 0) {
				// jを0に
				j = 0;
			}

			switch (actionWayPoint_[i]) {
			case SparkEffect::WayPoint1:
				// パーティクルの継続時間の間処理を行う
				if (particleT_[i] <= particleActiveTime_[i]) {
					// 子粒子は親粒子に同期
					worldTransformChildParticle_[j].translation_ =
					    worldTransformParticle_[i].translation_;
				} else {
					// 子粒子のパーティクルの速度を設定
					velocityChildParticle_[j] = {
					    MyMath::RandomF(-1.0f, 1.0f, 2), MyMath::RandomF(-1.0f, 1.0f, 2),
					    MyMath::RandomF(-1.0f, 1.0f, 2)};
					velocityChildParticle_[j] = velocityChildParticle_[j] * 3.0f;
					// 子粒子を表示
					isEndChildParticle_[j] = false;
				}
				break;
			case SparkEffect::WayPoint2:
				// パーティクルの継続時間の間処理を行う
				if (particleT_[i] <= particleActiveTime_[i]) {
					// 子粒子のパーティクルを移動
					worldTransformChildParticle_[j].translation_ =
					    worldTransformChildParticle_[j].translation_ + velocityChildParticle_[j];
				} else {
					// 終了状態に
					isEndChildParticle_[j] = true;
				}
				break;
			}

			if (!isEndChildParticle_[j]) {
				// ワールド座標の更新
				worldTransformChildParticle_[j].UpdateMatrix();
			}

		}
	}

	// 基底クラス更新
	BaseEffect::Update(); 
}

void SparkEffect::Draw(const ViewProjection& viewProjection) {
	// 粒子ごとに描画
	for (int i = 0; i < kMaxParticleCount_; i++) {
		if (!isEndParticle_[i]) {
			models_[0]->Draw(worldTransformParticle_[i], viewProjection);
			// 子粒子の初期化
			for (int j = (i * kMaxChildParticleCount_ - 1); j < kMaxChildParticleCount_ * (i + 1);
			     j++) {
				// jが0以下なら
				if (j < 0) {
					// jを0に
					j = 0;
				}
				if (!isEndChildParticle_[j])
					models_[1]->Draw(worldTransformChildParticle_[j], viewProjection);
			}
		}
	}
}
