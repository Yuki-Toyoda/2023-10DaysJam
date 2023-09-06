#pragma once
#include <stdint.h>
#include "Vector2.h"
#include "Vector3.h"
#include "ColliderShape/ColliderShape.h"

/// <summary>
/// 衝突判定クラス
/// </summary>
class Collider {

public: //デストラクタ
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Collider();

public: // アクセッサ等

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

	ColliderShape* GetColliderShape() { return colliderShape_; }

public: // その他関数

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	virtual void OnCollision() = 0;

protected: // 継承先メンバ変数

	// 衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;

	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;

	//形
	ColliderShape* colliderShape_;

};
