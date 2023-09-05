#include "BaseCharacter.h"

void BaseCharacter::Initialize(const std::vector<Model*>& models) {

	// モデルデータ配列
	models_ = models;
	// ワールド変換データの初期化
	worldTransform_.Initialize();
}

void BaseCharacter::Update() {

	// ワールド変換データの行列更新
	worldTransform_.UpdateMatrix();
}

void BaseCharacter::Draw(const ViewProjection& viewProjection) {
	// モデル描画
	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}
}

/// <summary>
/// ワールド座標取得
/// </summary>
/// <returns></returns>
Vector3 BaseCharacter::GetWorldPosition() {

	return Vector3{
	    worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1],
	    worldTransform_.matWorld_.m[3][2]};
}

void BaseCharacter::ApplyGlobalVariables() {
}
