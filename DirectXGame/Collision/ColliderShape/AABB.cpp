#include "AABB.h"

void AABB::Initialize(Vector3 min, Vector3 max) {

	colliderType_ = ColliderType::AABB;

	min_ = min;
	max_ = max;

}

void AABB::Update(Vector3 min, Vector3 max) {

	min_ = min;
	max_ = max;

}
