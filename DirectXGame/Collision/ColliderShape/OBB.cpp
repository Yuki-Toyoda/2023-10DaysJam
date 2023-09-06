#include "OBB.h"
#include "Matrix4x4.h"
#include "MyMath.h"
#include <PrimitiveDrawer.h>
#include "../config/GlobalVariables.h"

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

void OBB::Draw() {

	Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};

	Vector3 vertex[8];

	vertex[0] = {
	    center_.x + (-size_.x * otientatuons_[0].x + size_.y * otientatuons_[0].y +
	                 -size_.z * otientatuons_[0].z),
	    center_.y + (-size_.x * otientatuons_[1].x + size_.y * otientatuons_[1].y +
	                 -size_.z * otientatuons_[1].z),
	    center_.z + (-size_.x * otientatuons_[2].x + size_.y * otientatuons_[2].y +
	                 -size_.z * otientatuons_[2].z)};

	vertex[1] = {
	    center_.x + (-size_.x * otientatuons_[0].x + size_.y * otientatuons_[0].y +
	                    size_.z * otientatuons_[0].z),
	    center_.y + (-size_.x * otientatuons_[1].x + size_.y * otientatuons_[1].y +
	                    size_.z * otientatuons_[1].z),
	    center_.z + (-size_.x * otientatuons_[2].x + size_.y * otientatuons_[2].y +
	                    size_.z * otientatuons_[2].z)};

	vertex[2] = {
	    center_.x + (size_.x * otientatuons_[0].x + size_.y * otientatuons_[0].y +
	                    -size_.z * otientatuons_[0].z),
	    center_.y + (size_.x * otientatuons_[1].x + size_.y * otientatuons_[1].y +
	                    -size_.z * otientatuons_[1].z),
	    center_.z + (size_.x * otientatuons_[2].x + size_.y * otientatuons_[2].y +
	                    -size_.z * otientatuons_[2].z)};

	vertex[3] = {
	    center_.x + (size_.x * otientatuons_[0].x + size_.y * otientatuons_[0].y +
	                    size_.z * otientatuons_[0].z),
	    center_.y + (size_.x * otientatuons_[1].x + size_.y * otientatuons_[1].y +
	                    size_.z * otientatuons_[1].z),
	    center_.z + (size_.x * otientatuons_[2].x + size_.y * otientatuons_[2].y +
	                    size_.z * otientatuons_[2].z)};

	vertex[4] = {
	    center_.x + (-size_.x * otientatuons_[0].x + -size_.y * otientatuons_[0].y +
	                    -size_.z * otientatuons_[0].z),
	    center_.y + (-size_.x * otientatuons_[1].x + -size_.y * otientatuons_[1].y +
	                    -size_.z * otientatuons_[1].z),
	    center_.z + (-size_.x * otientatuons_[2].x + -size_.y * otientatuons_[2].y +
	                    -size_.z * otientatuons_[2].z)};

	vertex[5] = {
	    center_.x + (-size_.x * otientatuons_[0].x + -size_.y * otientatuons_[0].y +
	                    size_.z * otientatuons_[0].z),
	    center_.y + (-size_.x * otientatuons_[1].x + -size_.y * otientatuons_[1].y +
	                    size_.z * otientatuons_[1].z),
	    center_.z + (-size_.x * otientatuons_[2].x + -size_.y * otientatuons_[2].y +
	                    size_.z * otientatuons_[2].z)};

	vertex[6] = {
	    center_.x + (size_.x * otientatuons_[0].x + -size_.y * otientatuons_[0].y +
	                    -size_.z * otientatuons_[0].z),
	    center_.y + (size_.x * otientatuons_[1].x + -size_.y * otientatuons_[1].y +
	                    -size_.z * otientatuons_[1].z),
	    center_.z + (size_.x * otientatuons_[2].x + -size_.y * otientatuons_[2].y +
	                    -size_.z * otientatuons_[2].z)};

	vertex[7] = {
	    center_.x + (size_.x * otientatuons_[0].x + -size_.y * otientatuons_[0].y +
	                    size_.z * otientatuons_[0].z),
	    center_.y + (size_.x * otientatuons_[1].x + -size_.y * otientatuons_[1].y +
	                    size_.z * otientatuons_[1].z),
	    center_.z + (size_.x * otientatuons_[2].x + -size_.y * otientatuons_[2].y +
	                    size_.z * otientatuons_[2].z)};

	PrimitiveDrawer* primitiveDrawer = PrimitiveDrawer::GetInstance();

	primitiveDrawer->DrawLine3d(vertex[0], vertex[1], color);
	primitiveDrawer->DrawLine3d(vertex[0], vertex[2], color);
	primitiveDrawer->DrawLine3d(vertex[1], vertex[3], color);
	primitiveDrawer->DrawLine3d(vertex[2], vertex[3], color);

	primitiveDrawer->DrawLine3d(vertex[4], vertex[5], color);
	primitiveDrawer->DrawLine3d(vertex[4], vertex[6], color);
	primitiveDrawer->DrawLine3d(vertex[5], vertex[7], color);
	primitiveDrawer->DrawLine3d(vertex[6], vertex[7], color);

	primitiveDrawer->DrawLine3d(vertex[0], vertex[4], color);
	primitiveDrawer->DrawLine3d(vertex[1], vertex[5], color);
	primitiveDrawer->DrawLine3d(vertex[2], vertex[6], color);
	primitiveDrawer->DrawLine3d(vertex[3], vertex[7], color);

}

void OBB::AddToGlobalVariables(const char* groupName) {

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	// メンバ変数の調整したい項目をグローバル変数に追加
	globalVariables->AddItem(groupName, "OBBSize", size_);

}

void OBB::ApplyGlobalVariables(const char* groupName) {

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	// メンバ変数の調整項目をグローバル変数に追加
	size_ = globalVariables->GetVector3Value(groupName, "OBBSize");

}
