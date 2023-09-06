#include "AABB.h"
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
