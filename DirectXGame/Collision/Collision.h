#pragma once
#include "Collision/ColliderShape/ColliderShape.h"
#include "Collision/ColliderShape/Sphere.h"
#include "Collision/ColliderShape/AABB.h"
#include "Collision/ColliderShape/OBB.h"
class Collision {

public: //メンバ関数
	
	//球と球
	static bool IsCollisionSphereSphere(ColliderShape* ShapeA, ColliderShape* ShapeB);

	//球とAABB
	static bool IsCollisionSphereAABB(ColliderShape* ShapeA, ColliderShape* ShapeB);

	//球とOBB
	static bool IsCollisionSphereOBB(ColliderShape* ShapeA, ColliderShape* ShapeB);

	//OBBとOBB
	static bool IsCollisionOBBOBB(ColliderShape* ShapeA, ColliderShape* ShapeB);

};
