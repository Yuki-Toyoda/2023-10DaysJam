#pragma once
#include <stdint.h>
#include "Vector2.h"
#include "Vector3.h"

/// <summary>
/// 衝突判定クラス
/// </summary>
class Collider {
public: // サブクラス
	// 判定タイプ
	enum ColliderType {
		Sphere,
		Box
	};
public: // アクセッサ等

	/// <summary>
	/// 当たり判定半径ゲッター(球)
	/// </summary>
	/// <returns>当たり判定半径(球)</returns>
	float GetRadius() { return radius_; }
	/// <summary>
	/// 当たり判定半径セッター
	/// </summary>
	/// <param name="radius">当たり判定半径(球)</param>
	void SetRadius(float radius) { radius_ = radius; }

	/// <summary>
	/// 当たり判定サイズゲッター(箱)
	/// </summary>
	/// <returns>当たり判定サイズ(箱)</returns>
	Vector2 GetBoxRadius() { return boxRadius_; }
	/// <summary>
	/// 当たり判定サイズセッター(箱)
	/// </summary>
	/// <param name="radius">当たり判定サイズ(箱)</param>
	void SetBoxRadius(Vector2 radius) { boxRadius_ = radius; }

	/// <summary>
	/// 当たり判定タイプゲッター
	/// </summary>
	/// <returns>当たり判定タイプ</returns>
	ColliderType GetColliderType() { return colliderType_; }
	/// <summary>
	/// 当たり判定タイプセッター
	/// </summary>
	/// <param name="colliderType">当たり判定タイプ</param>
	void SetColliderType(ColliderType colliderType) { colliderType_ = colliderType; }

	/// <summary>
	/// ワールド座標取得
	/// </summary>
	/// <returns>ワールド座標</returns>
	virtual Vector3 GetWorldPosition() = 0;

	/// <summary>
	/// 衝突属性ゲッター(自分)
	/// </summary>
	/// <returns>衝突属性(自分)</returns>
	uint32_t GetCollisionAttribute() { return collisionAttribute_; }
	/// <summary>
	/// 衝突属性セッター(自分)
	/// </summary>
	/// <param name="collisionAttribute">衝突属性(自分)</param>
	void SetCollisionAttribute(uint32_t collisionAttribute) {
		collisionAttribute_ = collisionAttribute;
	}
	/// <summary>
	/// 衝突マスクゲッター(相手)
	/// </summary>
	/// <returns>衝突マスク(自分)</returns>
	uint32_t GetCollisionMask() { return collisionMask_; }
	/// <summary>
	/// 衝突マスクセッター(相手)
	/// </summary>
	/// <param name="collisionMask">衝突マスク(相手)</param>
	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }

public: // その他関数

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	virtual void OnCollision() = 0;

protected: // 継承先メンバ変数
	// 衝突半径(球)
	float radius_ = 1.0f;
	// 衝突判定(箱)
	Vector2 boxRadius_ = {1.0f, 1.0f};

	// 当たり判定種類
	ColliderType colliderType_;

	// 衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;

	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;

};
