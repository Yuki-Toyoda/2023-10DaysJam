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
	    obbCenter.x, obbCenter.y, obbCenter.z, 1};

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

bool Collision::IsCollisionOBBOBB(ColliderShape* ShapeA, ColliderShape* ShapeB) {

	Vector3* otientatuonsA = ShapeA->GetOtientatuons();
	Vector3 CenterA = ShapeA->GetCenter();
	Vector3 SizeA = ShapeA->GetSize();

	Vector3* otientatuonsB = ShapeB->GetOtientatuons();
	Vector3 CenterB = ShapeB->GetCenter();
	Vector3 SizeB = ShapeB->GetSize();
	
	// 軸
	Vector3 axis[15];

	// 面法線
	axis[0] = otientatuonsA[0];
	axis[1] = otientatuonsA[1];
	axis[2] = otientatuonsA[2];
	axis[3] = otientatuonsB[0];
	axis[4] = otientatuonsB[1];
	axis[5] = otientatuonsB[2];

	// クロス積
	axis[6] = MyMath::Cross(otientatuonsA[0], otientatuonsB[0]);
	axis[7] = MyMath::Cross(otientatuonsA[0], otientatuonsB[1]);
	axis[8] = MyMath::Cross(otientatuonsA[0], otientatuonsB[2]);
	axis[9] = MyMath::Cross(otientatuonsA[1], otientatuonsB[0]);
	axis[10] = MyMath::Cross(otientatuonsA[1], otientatuonsB[1]);
	axis[11] = MyMath::Cross(otientatuonsA[1], otientatuonsB[2]);
	axis[12] = MyMath::Cross(otientatuonsA[2], otientatuonsB[0]);
	axis[13] = MyMath::Cross(otientatuonsA[2], otientatuonsB[1]);
	axis[14] = MyMath::Cross(otientatuonsA[2], otientatuonsB[2]);

	// 頂点

	// 回転させる
	Vector3 obbVertex[8];

	obbVertex[0] = {+SizeA.x, +SizeA.y, +SizeA.z};

	obbVertex[1] = {+SizeA.x, +SizeA.y, -SizeA.z};

	obbVertex[2] = {+SizeA.x, -SizeA.y, +SizeA.z};

	obbVertex[3] = {+SizeA.x, -SizeA.y, -SizeA.z};

	obbVertex[4] = {-SizeA.x, +SizeA.y, +SizeA.z};

	obbVertex[5] = {-SizeA.x, +SizeA.y, -SizeA.z};

	obbVertex[6] = {-SizeA.x, -SizeA.y, +SizeA.z};

	obbVertex[7] = {-SizeA.x, -SizeA.y, -SizeA.z};

	Matrix4x4 obbRotateMatrix = {
	    otientatuonsA[0].x,
	    otientatuonsA[0].y,
	    otientatuonsA[0].z,
	    0.0f,
	    otientatuonsA[1].x,
	    otientatuonsA[1].y,
	    otientatuonsA[1].z,
	    0.0f,
	    otientatuonsA[2].x,
	    otientatuonsA[2].y,
	    otientatuonsA[2].z,
	    0.0f,
	    0.0f,
	    0.0f,
	    0.0f,
	    1.0f};

	for (int i = 0; i < 8; i++) {

		obbVertex[i] = MyMath::Transform(obbVertex[i], obbRotateMatrix);
		obbVertex[i] = obbVertex[i] + CenterA;
	}

	Vector3 obb2Vertex[8];

	obb2Vertex[0] = {+SizeB.x, +SizeB.y, +SizeB.z};

	obb2Vertex[1] = {+SizeB.x, +SizeB.y, -SizeB.z};

	obb2Vertex[2] = {+SizeB.x, -SizeB.y, +SizeB.z};

	obb2Vertex[3] = {+SizeB.x, -SizeB.y, -SizeB.z};

	obb2Vertex[4] = {-SizeB.x, +SizeB.y, +SizeB.z};

	obb2Vertex[5] = {-SizeB.x, +SizeB.y, -SizeB.z};

	obb2Vertex[6] = {-SizeB.x, -SizeB.y, +SizeB.z};

	obb2Vertex[7] = {-SizeB.x, -SizeB.y, -SizeB.z};

	Matrix4x4 obb2RotateMatrix = {
	    otientatuonsB[0].x,
	    otientatuonsB[0].y,
	    otientatuonsB[0].z,
	    0.0f,
	    otientatuonsB[1].x,
	    otientatuonsB[1].y,
	    otientatuonsB[1].z,
	    0.0f,
	    otientatuonsB[2].x,
	    otientatuonsB[2].y,
	    otientatuonsB[2].z,
	    0.0f,
	    0.0f,
	    0.0f,
	    0.0f,
	    1.0f};

	for (int i = 0; i < 8; i++) {

		obb2Vertex[i] = MyMath::Transform(obb2Vertex[i], obb2RotateMatrix);
		obb2Vertex[i] = obb2Vertex[i] + CenterB;
	}

	// 1.頂点を軸に対して射影
	for (int a = 0; a < 15; a++) {
		float min1 = 0.0f;
		float max1 = 0.0f;
		float min2 = 0.0f;
		float max2 = 0.0f;

		for (int v = 0; v < 8; v++) {
			// 一時保存
			float tmp = 0.0f;
			// obb
			tmp = MyMath::Dot(MyMath::Normalize(axis[a]), obbVertex[v]);
			// 2.射影した点の最大値と最小値を求める
			if (v == 0 || min1 > tmp) {
				min1 = tmp;
			}
			if (v == 0 || max1 < tmp) {
				max1 = tmp;
			}

			// obb2
			tmp = MyMath::Dot(MyMath::Normalize(axis[a]), obb2Vertex[v]);
			// 2.射影した点の最大値と最小値を求める
			if (v == 0 || min2 > tmp) {
				min2 = tmp;
			}
			if (v == 0 || max2 < tmp) {
				max2 = tmp;
			}
		}

		// 3.差分の形状を分離軸に射影した長さ
		float L1 = max1 - min1;
		float L2 = max2 - min2;

		float sumSpan = L1 + L2;
		float LonSpan = (std::max)(max1, max2) - (std::min)(min1, min2);
		if (sumSpan < LonSpan) {
			// 分離しているので分離軸
			return false;
		}
	}

	return true;

}
