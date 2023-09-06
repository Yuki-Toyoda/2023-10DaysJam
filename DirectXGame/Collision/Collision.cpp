#include "Collision.h"
#include "Matrix4x4.h"
#include "MyMath.h"
#include <algorithm>

bool Collision::IsCollisionSphereSphere(ColliderShape* ShapeA, ColliderShape* ShapeB) {

	// ワールド座標を取得
	Vector3 posA = ShapeA->GetCenter();
	Vector3 posB = ShapeB->GetCenter();
	
	// 座標AとBの距離を求める
	float distance = std::sqrtf(
	    std::powf(posB.x - posA.x, 2.0f) + std::powf(posB.y - posA.y, 2.0f) +
	    std::powf(posB.z - posA.z, 2.0f));
	
	// 球と球の交差判定
	if (distance <= ShapeA->GetRadius() + ShapeB->GetRadius()) {
		return true;
	}

	return false;

}

bool Collision::IsCollisionSphereAABB(ColliderShape* ShapeA, ColliderShape* ShapeB) {
	
	Vector3 sphereCenter = ShapeA->GetCenter();
	float sphereRadius = ShapeA->GetRadius();

	Vector3 AABBMin = ShapeB->GetMin();
	Vector3 AABBMax = ShapeB->GetMax();

	// 最近接点を求める
	Vector3 closestPoint{
	    std::clamp(sphereCenter.x, AABBMin.x, AABBMax.x),
	    std::clamp(sphereCenter.y, AABBMin.y, AABBMax.y),
	    std::clamp(sphereCenter.z, AABBMin.z, AABBMax.z)};

	// 最近接点と球の中心との距離を求める
	float distance = MyMath::Length(closestPoint - sphereCenter);
	// 距離が半径よりも小さければ衝突
	if (distance <= sphereRadius) {
		return true;
	}

	return false;

}

bool Collision::IsCollisionSphereOBB(ColliderShape* ShapeA, ColliderShape* ShapeB) { 
	
	Vector3 sphereCenter = ShapeA->GetCenter();
	float sphereRadius = ShapeA->GetRadius();

	Vector3* otientatuons = ShapeB->GetOtientatuons();
	Vector3 obbCenter = ShapeB->GetCenter();
	Vector3 obbSize = ShapeB->GetSize();

	Matrix4x4 obbWorldMatrix = {
	    otientatuons[0].x, otientatuons[1].x, otientatuons[2].x, 0,
	    otientatuons[0].y, otientatuons[1].y, otientatuons[2].y, 0,
	    otientatuons[0].z, otientatuons[1].z, otientatuons[2].z, 0,
	    obbCenter.x, obbCenter.y, obbCenter.y, 1};

	Matrix4x4 obbWorldMatrixInverse = MyMath::Inverse(obbWorldMatrix);

	Vector3 centerInOBBLocalSpace = MyMath::Transform(sphereCenter, obbWorldMatrixInverse);

	AABB aabbOBBLocal;
	aabbOBBLocal.Initialize(
	    Vector3(-obbSize.x, -obbSize.y, -obbSize.z), Vector3(obbSize.x, obbSize.y, obbSize.z));

	Sphere sphereOBBLocal;
	sphereOBBLocal.Initialize(centerInOBBLocalSpace, sphereRadius);

	// ローカル空間で衝突
	return IsCollisionSphereAABB(&sphereOBBLocal, &aabbOBBLocal);

}
