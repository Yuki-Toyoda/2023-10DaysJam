#include "AABB.h"
#include <PrimitiveDrawer.h>
#include "../config/GlobalVariables.h"

void AABB::Initialize(Vector3 min, Vector3 max) {

	colliderType_ = ColliderType::AABB;

	min_ = min;
	max_ = max;

}

void AABB::Update(Vector3 min, Vector3 max) {

	min_ = min;
	max_ = max;

}

void AABB::Draw() {

	Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};

	Vector3 vertex[8];

	vertex[0] = {min_.x, max_.y, min_.z};
	vertex[1] = {min_.x, max_.y, max_.z};
	vertex[2] = {max_.x, max_.y, min_.z};
	vertex[3] = {max_.x, max_.y, max_.z};

	vertex[4] = {min_.x, min_.y, min_.z};
	vertex[5] = {min_.x, min_.y, max_.z};
	vertex[6] = {max_.x, min_.y, min_.z};
	vertex[7] = {max_.x, min_.y, max_.z};

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

void AABB::AddToGlobalVariables(const char* groupName) {
	
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	// メンバ変数の調整したい項目をグローバル変数に追加
	globalVariables->AddItem(groupName, "AABBMin", min_);
	globalVariables->AddItem(groupName, "AABBMax", max_);

}

void AABB::ApplyGlobalVariables(const char* groupName) {

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	// メンバ変数の調整項目をグローバル変数に追加
	min_ = globalVariables->GetVector3Value(groupName, "AABBMin");
	max_ = globalVariables->GetVector3Value(groupName, "AABBMax");

}
