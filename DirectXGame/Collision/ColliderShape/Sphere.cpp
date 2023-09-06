#include "Sphere.h"

void Sphere::Initialize(Vector3 posision, float radius) {

	colliderType_ = ColliderType::Sphere;

	center_ = posision;
	radius_ = radius;

}

void Sphere::Update(Vector3 posision, float radius) {

	center_ = posision;
	radius_ = radius;

}
