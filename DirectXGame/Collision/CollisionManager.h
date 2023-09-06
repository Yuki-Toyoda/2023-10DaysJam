#include "Collider.h"
#include "Vector3.h"
#include <list>
#include "Collision/ColliderShape/ColliderShape.h"
#include "Collision/ColliderShape/Sphere.h"

/// <summary>
/// 衝突判定マネージャー
/// </summary>
class CollisionManager {
public: // メンバ関数
	/// <summary>
	/// リストクリア関数
	/// </summary>
	void ListClear();

	/// <summary>
	/// リストにコライダーを登録する関数
	/// </summary>
	/// <param name="collider">登録するコライダー</param>
	void ListRegister(Collider* collider);

	/// <summary>
	/// 全ての衝突判定を検証する関数
	/// </summary>
	void CheakAllCollision();

private: // メンバ関数
	/// <summary>
	/// 球同士の当たり判定を検証する関数
	/// </summary>
	/// <param name="colliderA">球A</param>
	/// <param name="colliderB">球B</param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

private: // 衝突判定

	/// <summary>
	/// どのあたり判定か
	/// </summary>
	/// <param name="ShapeA">コライダーA</param>
	/// <param name="ShapeB">コライダーB</param>
	void WhichCollision(Collider* colliderA, Collider* colliderB);

private: // メンバ変数
	// コライダーリスト
	std::list<Collider*> colliders_;
};