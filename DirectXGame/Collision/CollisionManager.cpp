#include "CollisionManager.h"
#include <cmath>
#include "Collision.h"

// リストのクリア
void CollisionManager::ListClear() { colliders_.clear(); }

// リスト登録
void CollisionManager::ListRegister(Collider* collider) { colliders_.push_back(collider); }

// 当たり判定処理
// 衝突判定と応答
void CollisionManager::CheakAllCollision() {

	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得する
		Collider* colliderA = *itrA;
		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			// イテレータBからコライダーBを取得する
			Collider* colliderB = *itrB;

			// ペアの当たり判定
			if (CheckCollisionPair(colliderA, colliderB)) {
				colliders_.remove(colliderB);
				break;
			}

		}
	}
}

// コライダー2つの衝突判定と応答
bool CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {

	// 衝突フィルタリング
	if (!(colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) ||
	    !(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask())) {
		return false;
	}

	//あたり判定タイプ確認
	return WhichCollision(colliderA, colliderB);

	

}

bool CollisionManager::WhichCollision(Collider* colliderA, Collider* colliderB) {

	//あたったか
	bool isCollision = false;

	// 球のあたり判定
	if (colliderA->GetColliderShape()->GetColliderType() == ColliderShape::ColliderType::Sphere) {
		// 球のあたり判定
		if (colliderB->GetColliderShape()->GetColliderType() == ColliderShape::ColliderType::Sphere) {
			isCollision = Collision::IsCollisionSphereSphere(colliderA->GetColliderShape(), colliderB->GetColliderShape());
		} 
		// AABBのあたり判定
		else if (colliderB->GetColliderShape()->GetColliderType() == ColliderShape::ColliderType::AABB) {
			isCollision = Collision::IsCollisionSphereAABB(colliderA->GetColliderShape(), colliderB->GetColliderShape());
		}
		// OBBのあたり判定
		else if (colliderB->GetColliderShape()->GetColliderType() == ColliderShape::ColliderType::OBB) {
			isCollision = Collision::IsCollisionSphereOBB(colliderA->GetColliderShape(), colliderB->GetColliderShape());
		}

	} 
	// AABBのあたり判定
	else if (colliderA->GetColliderShape()->GetColliderType() == ColliderShape::ColliderType::AABB) {
		// 球のあたり判定
		if (colliderB->GetColliderShape()->GetColliderType() == ColliderShape::ColliderType::Sphere) {
			isCollision = Collision::IsCollisionSphereAABB(colliderB->GetColliderShape(), colliderA->GetColliderShape());
		} 
	}
	// OBBのあたり判定
	else if (
	    colliderA->GetColliderShape()->GetColliderType() == ColliderShape::ColliderType::OBB) {
		// 球のあたり判定
		if (colliderB->GetColliderShape()->GetColliderType() == ColliderShape::ColliderType::Sphere) {
			isCollision = Collision::IsCollisionSphereOBB(colliderB->GetColliderShape(), colliderA->GetColliderShape());
		}
		// OBBのあたり判定
		else if (colliderB->GetColliderShape()->GetColliderType() == ColliderShape::ColliderType::OBB) {
			isCollision = Collision::IsCollisionOBBOBB(colliderA->GetColliderShape(), colliderB->GetColliderShape());
		}
	}

	//衝突時の動作
	if (isCollision) {
		colliderA->OnCollision(colliderB);
		colliderB->OnCollision(colliderA);
		return true;
	}

	return false;

}
