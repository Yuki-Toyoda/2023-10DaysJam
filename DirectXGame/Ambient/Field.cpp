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

void Field::ApplyGlobalVariables() {

	// 調整項目クラスのインスタンス取得
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	// グループ名の設定
	const char* groupName = "Field";

	// メンバ変数の調整項目をグローバル変数に追加
	min = globalVariables->GetVector3Value(groupName, "Min");
	max = globalVariables->GetVector3Value(groupName, "Max");

}
