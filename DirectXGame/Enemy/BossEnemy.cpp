#include "BossEnemy.h"
#include "MyMath.h"
#include "Charactor/Player/Player.h"
#include "../config/GlobalVariables.h"
#include <cassert>
#include <stdio.h>
#include <string>

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

}

/// <summary>
/// 更新
/// </summary>
void BossEnemy::Update() {

	Move();
	Rotation();

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
void BossEnemy::OnCollision(Tag collisionTag) { 
	
	if (collisionTag == TagPlayer ||
		collisionTag == TagPlayerBulletFire ||
		collisionTag == TagPlayerBulletIce ||
		collisionTag == TagPlayerBulletThunder ||
	    collisionTag == TagPlayerBulletNone	) {
		isDead_ = true; 
	}

}

void BossEnemy::Move() {

	//位置設定
	Vector3 startPoint = movePoint[movePointNum];
	Vector3 endPoint;
	if (movePointNum == movePointMax - 1) {
		endPoint = movePoint[0];
	} else {
		endPoint = movePoint[movePointNum + 1];
	}

	//移動
	worldTransform_.translation_ = MyMath::Linear(moveT, startPoint, endPoint, moveTime);

	//moveTについて
	moveT += 1.0f / 60.0f;
	if (moveT >= moveTime) {
		moveT = 0.0f;
		movePointNum++;
		if (movePointNum == movePointMax) {
			movePointNum = 0;
		}
	}

}

void BossEnemy::Rotation() {

	// 自キャラのワールド座標を取得する
	Vector3 playerPos = player_->GetWorldPosition();
	// 敵弾のワールド座標を取得する
	Vector3 enemyrPos = GetWorldPosition();
	// 敵弾->自キャラの差分ベクトルを求める
	Vector3 toPlayer = playerPos - enemyrPos;
	// ベクトルの正規化
	toPlayer = MyMath::Normalize(toPlayer);
	// 進行方向に見た目の回転を合わせる
	//  Y軸周りの角度(Θy)
	worldTransform_.rotation_.y = std::atan2f(toPlayer.x, toPlayer.z);
	// 横軸方向の長さを求める
	float length = MyMath::Length(Vector3{toPlayer.x, 0.0f, toPlayer.z});
	// X軸周りの角度(Θx)
	worldTransform_.rotation_.x = std::atan2f(-toPlayer.y, length);

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

}
