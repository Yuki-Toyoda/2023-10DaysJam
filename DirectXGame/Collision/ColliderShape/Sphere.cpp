#include "Sphere.h"

void Sphere::Initialize(Vector3 center, float radius) {

	colliderType_ = ColliderType::Sphere;

	center_ = center;
	radius_ = radius;

}

void Sphere::Update(Vector3 posision) {

	//ワールドポジションを代入
	center_ = posision;

}
