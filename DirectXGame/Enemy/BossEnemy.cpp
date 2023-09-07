#include "BossEnemy.h"
#include "MyMath.h"
#include "Charactor/Player/Player.h"
#include "../config/GlobalVariables.h"
#include <cassert>
#include <stdio.h>
#include <string>
#include "Enemy.h"
#include "Collision/ColliderShape/OBB.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="models">モデルデータ配列</param>
void BossEnemy::Initialize(const std::vector<Model*>& models, uint32_t textureHandle, std::list<Enemy*>* enemies) {

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

	//エネミー
	enemies_ = enemies;

	// 衝突属性を設定
	SetCollisionAttribute(0xfffffffb);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(0x00000004);

	// タグ
	tag_ = TagBossEnemy;

	// コライダーの形
	OBB* obb = new OBB();
	obb->Initialize(GetWorldPosition(), worldTransform_.rotation_, worldTransform_.scale_);
	colliderShape_ = obb;
	
	// 移動通過地点
	movePoint[0] = {-300, 10, -300};
	movePoint[1] = {-300, 10,  300};
	movePoint[2] = { 300, 10,  300};
	movePoint[3] = { 300, 10, -300};

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "BossEnemy";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加
	globalVariables->AddItem(groupName, "MovePointMax", int(movePointMax));
	for (size_t i = 0; i < movePointMax; i++) {
		char str[32];
		sprintf_s(str, "movePoint%d", int(i));
		globalVariables->AddItem(groupName, str, movePoint[i]);
	}
	globalVariables->AddItem(groupName, "MoveTime", moveTime);

	colliderShape_->AddToGlobalVariables(groupName);

}

/// <summary>
/// 更新
/// </summary>
void BossEnemy::Update(std::list<Enemy*>* enemies) {

	enemies_ = enemies;

	//Move();

	CollectEnemies();

	// ワールド行列更新
	worldTransform_.UpdateMatrix();

	// コライダー更新
	colliderShape_->Update(GetWorldPosition(), worldTransform_.rotation_, colliderShape_->GetSize());

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
void BossEnemy::OnCollision(Tag collisionTag) { 
	
	if (collisionTag == TagPlayer ||
		collisionTag == TagPlayerBulletFire ||
		collisionTag == TagPlayerBulletIce ||
		collisionTag == TagPlayerBulletThunder ||
	    collisionTag == TagPlayerBulletNone	) {
		//isDead_ = true; 
	}

}

void BossEnemy::Move() {

	

}

void BossEnemy::CollectEnemies() {

	//Wait状態の一番近いエネミーを検索

	//検索出来たか
	bool enemyFound = false;
	Vector3 shortestPos = {0.0f, 0.0f, 0.0f};
	float shortest = 0.0f;
	// ワールド座標を取得する
	Vector3 pos = GetWorldPosition();
	for (Enemy* enemy : *enemies_) {
		if (enemy) {
			if (enemy->GetEnemyState() == Enemy::Wait) {
				// ボスのワールド座標を取得する
				Vector3 enemyPos = enemy->GetWorldPosition();
				// 敵弾->自キャラの差分ベクトルを求める
				Vector3 toEnemy = pos - enemyPos;
				// 距離
				float distance = MyMath::Length(toEnemy);
				if (enemyFound == false || shortest >= distance) {
					shortest = distance;
					shortestPos = enemyPos;
					enemyFound = true;
				}
			}
		}
	}

	if (enemyFound) {
		//移動割合
		float t = 0.025f;
		worldTransform_.translation_ = {
		    MyMath::Linear(t, pos.x, shortestPos.x), MyMath::Linear(t, pos.y, shortestPos.y),
		    MyMath::Linear(t, pos.z, shortestPos.z)};
	} else {
		//通常移動
	}

}

void BossEnemy::ApplyGlobalVariables() {

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "BossEnemy";

	// メンバ変数の調整項目をグローバル変数に追加
	movePointMax = int(globalVariables->GetIntValue(groupName, "MovePointMax"));
	for (size_t i = 0; i < movePointMax; i++) {
		char str[32];
		sprintf_s(str, "movePoint%d", int(i));
		movePoint[i] = globalVariables->GetVector3Value(groupName, str);
	}
	moveTime = globalVariables->GetFloatValue(groupName, "MoveTime");

	colliderShape_->ApplyGlobalVariables(groupName);

}
