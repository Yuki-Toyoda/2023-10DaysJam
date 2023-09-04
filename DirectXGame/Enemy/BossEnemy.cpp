#include "BossEnemy.h"
#include <cassert>

/// <summary>
/// 初期化
/// </summary>
/// <param name="models">モデルデータ配列</param>
void BossEnemy::Initialize(const std::vector<Model*>& models, uint32_t textureHandle) {

	// NULLポインタチェック
	assert(models.front());

	// ベースキャラクター初期化
	BaseCharacter::Initialize(models);
	textureHandle_ = textureHandle;

	// 初期座標
	worldTransform_.translation_ = {10.0f, 0.0f, 30.0f};
	worldTransform_.UpdateMatrix();

	// デスフラグ
	isDead_ = false;

	// 速度
	velocity_ = {
	    0.0f,
	    0.0f,
	    0.0f,
	};

	// 衝突属性を設定
	SetCollisionAttribute(0xfffffffd);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(0x00000002);
}

/// <summary>
/// 更新
/// </summary>
void BossEnemy::Update() {

	// ワールド行列更新
	worldTransform_.UpdateMatrix();
}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
void BossEnemy::Draw(const ViewProjection& viewProjection) {

	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}
}

// 衝突時に呼ばれる関数
void BossEnemy::OnCollision() { isDead_ = true; }
