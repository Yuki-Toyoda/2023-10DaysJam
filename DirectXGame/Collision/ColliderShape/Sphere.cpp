#include "Sphere.h"
#include "../config/GlobalVariables.h"

void Sphere::Initialize(Vector3 posision, float radius) {

	colliderType_ = ColliderType::Sphere;

	center_ = posision;
	radius_ = radius;

}

void Sphere::Update(Vector3 posision, float radius) {

	center_ = posision;
	radius_ = radius;

}

void Sphere::AddToGlobalVariables(const char* groupName) {
	
	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	// メンバ変数の調整したい項目をグローバル変数に追加
	globalVariables->AddItem(groupName, "SphereRadius", radius_);

}

void Sphere::ApplyGlobalVariables(const char* groupName) {

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	// メンバ変数の調整項目をグローバル変数に追加
	radius_ = globalVariables->GetFloatValue(groupName, "SphereRadius");

}
