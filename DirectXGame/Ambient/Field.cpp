#include "Field.h"
#include"config/GlobalVariables.h"
#include <PrimitiveDrawer.h>

Field* Field::GetInstance() { 
	static Field instance;
	return &instance;
}

void Field::Initialize() {

	//min
	min = {-10000.0f, -10000.0f, -10000.0f};
	// max
	max = {10000.0f, 10000.0f, 10000.0f};

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名設定
	const char* groupName = "Field";
	// 指定した名前でグループ追加
	globalVariables->CreateGroup(groupName);

	// メンバ変数の調整したい項目をグローバル変数に追加
	globalVariables->AddItem(groupName, "Min", min);
	globalVariables->AddItem(groupName, "Max", max);

	ApplyGlobalVariables();

}

void Field::Update() {

	ApplyGlobalVariables();

}

void Field::Draw() {

	PrimitiveDrawer* primitiveDrawer = PrimitiveDrawer::GetInstance();

	Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};

	Vector3 vertex[8] = {};

	vertex[0] = {min.x, min.y, min.z};
	vertex[1] = {min.x, min.y, max.z};
	vertex[2] = {min.x, max.y, min.z};
	vertex[3] = {min.x, max.y, max.z};
	vertex[4] = {max.x, min.y, min.z};
	vertex[5] = {max.x, min.y, max.z};
	vertex[6] = {max.x, max.y, min.z};
	vertex[7] = {max.x, max.y, max.z};

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

void Field::ApplyGlobalVariables() {

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "Field";

	// メンバ変数の調整項目をグローバル変数に追加
	min = globalVariables->GetVector3Value(groupName, "Min");
	max = globalVariables->GetVector3Value(groupName, "Max");

}
