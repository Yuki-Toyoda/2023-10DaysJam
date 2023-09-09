#include "EnemyBullet.h"
#include <cassert>
#include "MyMath.h"
#include "Collision/ColliderShape/OBB.h"
#include "../config/GlobalVariables.h"

void EnemyBullet::Initialize(
    const std::vector<Model*>& models, const Vector3& position, const Vector3& velocity) {

	// NULLポインタチェック
	assert(models.front());

	// ベースキャラクター初期化
	BaseCharacter::Initialize(models);

	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	// Y軸周りの角度(Θy)
	worldTransform_.rotation_.y = std::atan2f(velocity_.x, velocity_.z);
	// 横軸方向の長さを求める
	float length = MyMath::Length(Vector3{velocity_.x, 0.0f, velocity_.z});
	// X軸周りの角度(Θx)
	worldTransform_.rotation_.x = std::atan2f(-velocity_.y, length);

	// 衝突属性を設定
	SetCollisionAttribute(0xfffffffd);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(0x00000002);
	// タグ
	tag_ = TagEnemyBullet;

	// コライダーの形
	OBB* obb = new OBB();
	obb->Initialize(GetWorldPosition(), worldTransform_.rotation_, worldTransform_.scale_);
	colliderShape_ = obb;

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "EnemyBullet";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);
	
	colliderShape_->AddToGlobalVariables(groupName);

}

void EnemyBullet::Update() {

	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ = worldTransform_.translation_ + velocity_;
	
	// 基底クラス更新
	BaseCharacter::Update();

	// コライダー更新
	colliderShape_->Update(GetWorldPosition(), worldTransform_.rotation_, worldTransform_.scale_);

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// グローバル変数適用
	ApplyGlobalVariables();

}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {

	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}

}

void EnemyBullet::OnCollision(Collider* collision) {

	if (collision->GetTag() != TagBossEnemy ) {
		isDead_ = true; 
	}

}

void EnemyBullet::ApplyGlobalVariables() {

	// グループ名の設定
	const char* groupName = "EnemyBullet";

	colliderShape_->ApplyGlobalVariables(groupName);

}
