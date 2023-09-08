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
void BossEnemy::Initialize(
    const std::vector<Model*>& models, uint32_t textureHandle, std::list<Enemy*>* enemies,
    Player* player, Vector3 colliderSize) {

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

	// エネミーの数
	enemiesJoiningNum = 0;
	// エネミーの最大数
	enemiesJoiningNumMax = 8;

	preAttackT_ = 0.0f;

	preAttackCooltime_ = preAttackCooltimeMax_;

	//エネミー
	enemies_ = enemies;
	//プレイヤー
	player_ = player;

	// 衝突属性を設定
	SetCollisionAttribute(0xfffffffb);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(0x00000004);

	// タグ
	tag_ = TagBossEnemy;

	// コライダーの形
	OBB* obb = new OBB();
	obb->Initialize(GetWorldPosition(), worldTransform_.rotation_, colliderSize);
	colliderShape_ = obb;

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "BossEnemy";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	globalVariables->AddItem(groupName, "MoveSpeed", moveSpeed_);
	globalVariables->AddItem(groupName, "MoveRotateSpeed", moveRotateSpeed_);

	// メンバ変数の調整したい項目をグローバル変数に追加
	colliderShape_->AddToGlobalVariables(groupName);

}

/// <summary>
/// 更新
/// </summary>
void BossEnemy::Update(std::list<Enemy*>* enemies) {

	enemies_ = enemies;
	
	switch (bossEnemyState_) {
	case BossEnemy::Collect:
		// エネミーの収集
		CollectEnemies();
		break;
	case BossEnemy::PreAttackCommand:
		PreAttack();
		break;
	case BossEnemy::AttackCommand:
		//攻撃中
		UnderAttack();
		break;
	case BossEnemy::Down:
		break;
	default:
		break;
	}

	// ワールド行列更新
	BaseCharacter::Update();

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
void BossEnemy::OnCollision(Collider* collision) { 
	
	if (collision->GetTag() == TagPlayer || collision->GetTag() == TagPlayerBulletFire ||
	    collision->GetTag() == TagPlayerBulletIce ||
	    collision->GetTag() == TagPlayerBulletThunder ||
	    collision->GetTag() == TagPlayerBulletNone) {
		//isDead_ = true; 
	}
	if (collision->GetTag() == TagEnemy) {
		enemiesJoiningNum++;
		if (enemiesJoiningNum == enemiesJoiningNumMax) {
			bossEnemyState_ = PreAttackCommand; 
		}
	}

}

void BossEnemy::Move() {

	// 回転
	// 回転速度
	Vector3 velocity(0.0f, moveRotateSpeed_, 0.0f);
	worldTransform_.rotation_ = worldTransform_.rotation_ + velocity;

	// 移動
	//  速度ベクトルを自機の向きに合わせて回転させる
	velocity = {0.0f, 0.0f, moveSpeed_};

	velocity = MyMath::Transform(velocity, MyMath::MakeRotateXYZMatrix(worldTransform_.rotation_));
	worldTransform_.translation_ = worldTransform_.translation_ + velocity;

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
		Vector3 toEnemy = MyMath::Normalize(shortestPos - pos);
		// 速度
		velocity_ = toEnemy * moveSpeed_;
		// 座標を移動させる(1フレーム分の移動量を足しこむ)
		worldTransform_.translation_ = worldTransform_.translation_ + velocity_;
		//回転
		MoveRotation(toEnemy);
	} else {
		//通常移動
		Move();
	}

}

void BossEnemy::PreAttack() {

	//イージング回転
	if (preAttackT_ < 1.0f) {
		Vector3 rotate = worldTransform_.rotation_;
		Vector3 target = worldTransform_.rotation_;
		float pi = float(std::numbers::pi);

		// 回転
		// 自キャラのワールド座標を取得する
		Vector3 playerPos = player_->GetWorldPosition();
		// 敵弾のワールド座標を取得する
		Vector3 bossEnemyrPos = GetWorldPosition();
		// 敵弾->自キャラの差分ベクトルを求める
		Vector3 toPlayer = playerPos - bossEnemyrPos;

		//  Y軸周りの角度(Θy)
		target.y = std::atan2f(toPlayer.x, toPlayer.z);
		// 横軸方向の長さを求める
		float length = MyMath::Length(Vector3{toPlayer.x, 0.0f, toPlayer.z});
		// X軸周りの角度(Θx)
		target.x = std::atan2f(-toPlayer.y, length);

		// ターゲット
		while (target.x > pi) {
			target.x -= pi;
		}
		while (target.x < -pi) {
			target.x += pi;
		}

		while (target.y > pi) {
			target.y -= pi;
		}
		while (target.y < -pi) {
			target.y += pi;
		}

		while (target.z > pi) {
			target.z -= pi;
		}
		while (target.z < -pi) {
			target.z += pi;
		}

		// rotate
		while (rotate.x > pi) {
			rotate.x -= pi;
		}
		while (rotate.x < -pi) {
			rotate.x += pi;
		}

		while (rotate.y > pi) {
			rotate.y -= pi;
		}
		while (rotate.y < -pi) {
			rotate.y += pi;
		}

		while (rotate.z > pi) {
			rotate.z -= pi;
		}
		while (rotate.z < -pi) {
			rotate.z += pi;
		}

		// 回転割合
		preAttackT_ += preAttackTSpeed_;
		if (preAttackT_ >= 1.0f) {
			preAttackT_ = 1.0f;
		}
		worldTransform_.rotation_ = {
		    MyMath::Linear(preAttackT_, rotate.x, target.x),
		    MyMath::Linear(preAttackT_, rotate.y, target.y),
		    rotate.z
		};
	}
	//イージング終了したら待機
	else {
		if (--preAttackCooltime_ == 0) {
			// 攻撃開始
			BeginAttack();
			preAttackCooltime_ = preAttackCooltimeMax_;
			preAttackT_ = 0.0f;
		}
	}

}

void BossEnemy::BeginAttack() {

	bossEnemyState_ = AttackCommand; 

	//クールタイム
	attackCooltime_ = attackCooltimeMax_;

	// 回転
	// 自キャラのワールド座標を取得する
	Vector3 playerPos = player_->GetWorldPosition();
	// 敵弾のワールド座標を取得する
	Vector3 bossEnemyrPos = GetWorldPosition();
	// 敵弾->自キャラの差分ベクトルを求める
	Vector3 toPlayer = playerPos - bossEnemyrPos;
	//  Y軸周りの角度(Θy)
	worldTransform_.rotation_.y = std::atan2f(toPlayer.x, toPlayer.z);
	// 横軸方向の長さを求める
	float length = MyMath::Length(Vector3{toPlayer.x, 0.0f, toPlayer.z});
	// X軸周りの角度(Θx)
	worldTransform_.rotation_.x = std::atan2f(-toPlayer.y, length);

}

void BossEnemy::UnderAttack() {

	if (--attackCooltime_ == 0) {
		bossEnemyState_ = Collect;
		enemiesJoiningNum = 0;
	}

}

void BossEnemy::MoveRotation(Vector3 toPosition) {

	Vector3 rotate = worldTransform_.rotation_;
	Vector3 target = worldTransform_.rotation_;
	float pi = float(std::numbers::pi);

	//  Y軸周りの角度(Θy)
	target.y = std::atan2f(toPosition.x, toPosition.z);
	// 横軸方向の長さを求める
	float length = MyMath::Length(Vector3{toPosition.x, 0.0f, toPosition.z});
	// X軸周りの角度(Θx)
	target.x = std::atan2f(-toPosition.y, length);

	//ターゲット
	while (target.x > pi) {
		target.x -= pi;
	}
	while (target.x < -pi) {
		target.x += pi;
	}

	while (target.y > pi) {
		target.y -= pi;
	}
	while (target.y < -pi) {
		target.y += pi;
	}

	while (target.z > pi) {
		target.z -= pi;
	}
	while (target.z < -pi) {
		target.z += pi;
	}

	
	// rotate
	while (rotate.x > pi) {
		rotate.x -= pi;
	}
	while (rotate.x < -pi) {
		rotate.x += pi;
	}

	while (rotate.y > pi) {
		rotate.y -= pi;
	}
	while (rotate.y < -pi) {
		rotate.y += pi;
	}

	while (rotate.z > pi) {
		rotate.z -= pi;
	}
	while (rotate.z < -pi) {
		rotate.z += pi;
	}

	// 回転割合
	float t = 0.05f;
	worldTransform_.rotation_ = {
	    MyMath::Linear(t, rotate.x, target.x), MyMath::Linear(t, rotate.y, target.y), rotate.z
	};

}

void BossEnemy::ApplyGlobalVariables() {

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "BossEnemy";

	// メンバ変数の調整項目をグローバル変数に追加
	moveSpeed_ = globalVariables->GetFloatValue(groupName, "MoveSpeed");
	moveRotateSpeed_ = globalVariables->GetFloatValue(groupName, "MoveRotateSpeed");

	colliderShape_->ApplyGlobalVariables(groupName);

}
