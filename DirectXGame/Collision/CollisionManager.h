#include "Collider.h"
#include "Vector3.h"
#include <list>

/// <summary>
/// 衝突判定マネージャー
/// </summary>
class CollisionManager {
public: // メンバ関数
	// リストのクリア
	void ListClear();

	// リスト登録
	void ListRegister(Collider* collider);

	// 衝突判定と応答
	void CheakAllCollision();

private: // メンバ変数
	// コライダーリスト
	std::list<Collider*> colliders_;

	// コライダー2つの衝突判定と応答
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
};