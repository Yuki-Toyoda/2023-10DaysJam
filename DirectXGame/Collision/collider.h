#pragma once
#include <stdint.h>
#include "Vector3.h"

/// <summary>
/// 衝突判定クラス
/// </summary>
class Collider {

public: // メンバ関数
	// 半径を取得
	float GetRadius() { return radius_; }
	// 半径を設定
	void SetRadius(float radius) { radius_ = radius; }

	/// <summary>
	/// ワールド座標取得
	/// </summary>
	/// <returns></returns>
	virtual Vector3 GetWorldPosition() = 0;

	// 衝突時に呼ばれる関数
	virtual void OnCollision() = 0;

	// 衝突属性(自分)を取得
	uint32_t GetCollisionAttribute() { return collisionAttribute_; }
	// 衝突属性(自分)を設定
	void SetCollisionAttribute(uint32_t collisionAttribute) {
		collisionAttribute_ = collisionAttribute;
	}
	// 衝突マスク(相手)を取得
	uint32_t GetCollisionMask() { return collisionMask_; }
	// 衝突マスク(相手)を設定
	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }

protected: // 継承先メンバ変数
	// 衝突半径
	float radius_ = 1.0f;
	// 衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;

	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;
};
