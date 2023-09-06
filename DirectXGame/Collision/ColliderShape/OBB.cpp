#include "OBB.h"
#include "Matrix4x4.h"
#include "MyMath.h"
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
