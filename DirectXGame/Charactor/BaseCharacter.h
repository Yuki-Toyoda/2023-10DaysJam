#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <memory>
#include <vector>
#include "../Collision/collider.h"

/// <summary>
/// キャラクター継承元
/// </summary>
class BaseCharacter : public Collider{
protected: // 継承先メンバ変数
	// モデルデータ配列
	std::vector<Model*> models_;
	// ワールド変換データ
	WorldTransform worldTransform_;

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデルデータ配列</param>
	virtual void Initialize(const std::vector<Model*>& models);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	virtual void Draw(const ViewProjection& viewProjection);

public: // アクセッサ等

	/// <summary>
	/// ワールド座標ゲッター
	/// </summary>
	/// <returns>ワールド座標</returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	/// <summary>
	/// ワールド座標取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	virtual void OnCollision(Collider* collision) { collision; };

public: // 自分の情報
	virtual Enemy* GetEnemy() override { return nullptr; }
	
	virtual BossEnemy* GetBossEnemy() override { return nullptr; }

	virtual Player* GetPlayer() override { return nullptr; }

	virtual PlayerBullet* GetPlayerBullet() override { return nullptr; }

public: // その他関数

	/// <summary>
	/// 調整項目適用関数
	/// </summary>
	virtual void ApplyGlobalVariables();

};
