#pragma once
#include "Vector3.h"
/// <summary>
/// コライダーの形
/// </summary>
class ColliderShape {

public: // サブクラス
	// 判定タイプ
	enum ColliderType {
		Sphere, 
		Box 
	};

public: // 継承メンバ関数
	virtual ~ColliderShape() {}

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

	//球
	virtual void Update(Vector3 posision) = 0;

	virtual float GetRadius() = 0;
	virtual Vector3 GetCenter() = 0;

protected: // 継承先メンバ変数
	
	// 当たり判定種類
	ColliderType colliderType_;

};
