#include "OBB.h"
#include "Matrix4x4.h"
#include "MyMath.h"

void OBB::Initialize(Vector3 position, Vector3 rotate, Vector3 size) {

	colliderType_ = ColliderType::OBB;

	center_ = position;

	Matrix4x4 rotateMatirx = MyMath::MakeRotateXYZMatrix(rotate);

	otientatuons_[0].x = rotateMatirx.m[0][0];
	otientatuons_[0].y = rotateMatirx.m[0][1];
	otientatuons_[0].z = rotateMatirx.m[0][2];

	otientatuons_[1].x = rotateMatirx.m[1][0];
	otientatuons_[1].y = rotateMatirx.m[1][1];
	otientatuons_[1].z = rotateMatirx.m[1][2];

	otientatuons_[2].x = rotateMatirx.m[2][0];
	otientatuons_[2].y = rotateMatirx.m[2][1];
	otientatuons_[2].z = rotateMatirx.m[2][2];

	size_ = size;

}

void OBB::Update(Vector3 position, Vector3 rotate, Vector3 size) {

    center_ = position;

	 Matrix4x4 rotateMatirx = MyMath::MakeRotateXYZMatrix(rotate);

	otientatuons_[0].x = rotateMatirx.m[0][0];
	otientatuons_[0].y = rotateMatirx.m[0][1];
	otientatuons_[0].z = rotateMatirx.m[0][2];

	otientatuons_[1].x = rotateMatirx.m[1][0];
	otientatuons_[1].y = rotateMatirx.m[1][1];
	otientatuons_[1].z = rotateMatirx.m[1][2];

	otientatuons_[2].x = rotateMatirx.m[2][0];
	otientatuons_[2].y = rotateMatirx.m[2][1];
	otientatuons_[2].z = rotateMatirx.m[2][2];

	size_ = size;

}
