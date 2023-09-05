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

	// 生存している状態に
	isDead_ = false;
	// 生存時間設定
	deathTimer_ = klifeTime_;

}

void PlayerBullet::Update() {
	// 弾をベクトルの方向に前進させる
	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;

	// 死亡タイマーデクリメント
	deathTimer_--;
	// 死亡タイマーが0以下になったら
	if (deathTimer_ <= 0) {
		// 弾を死亡させる
		isDead_ = true;
	}

	// 基底クラス更新
	BaseCharacter::Update();

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) { 
	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}
}

void PlayerBullet::OnCollision() {}
