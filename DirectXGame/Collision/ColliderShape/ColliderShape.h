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
		AABB,
		OBB
	};

public: // 継承メンバ関数
	
	/// <summary>
	/// デストラクタ
	/// </summary>
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
	virtual void Update(Vector3 posision, float radius);

	virtual float GetRadius() { return 0.0f; };
	virtual Vector3 GetCenter() { return Vector3(0.0f, 0.0f, 0.0f); };

	//AABB
	virtual void Update(Vector3 min, Vector3 max);

	virtual Vector3 GetMin() { return Vector3(0.0f, 0.0f, 0.0f); };
	virtual Vector3 GetMax() { return Vector3(0.0f, 0.0f, 0.0f); };

	//OBB
	virtual void Update(Vector3 posision, Vector3 rotate, Vector3 size);

	virtual Vector3* GetOtientatuons() { return nullptr; };
	virtual Vector3 GetSize() { return Vector3(0.0f, 0.0f, 0.0f); };

protected: // 継承先メンバ変数
	
	// 当たり判定種類
	ColliderType colliderType_;

};
