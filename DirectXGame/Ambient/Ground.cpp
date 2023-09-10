#include "Ground.h"
#include <cassert>

void Ground::Intialize(Model* model) {
	// NULLチェック
	assert(model);

	// 引数の値をメンバ変数に代入
	model_ = model;

	// ワールド座標初期化
	worldTransform_.Initialize();
	worldTransform_.scale_.x *= 2.5f;
	worldTransform_.scale_.z *= 2.5f;
	worldTransform_.UpdateMatrix();

}

void Ground::Update() {
	// 行列転送
	worldTransform_.TransferMatrix();
}

void Ground::Draw(const ViewProjection& viewProjection) {
	// モデル描画
	model_->Draw(worldTransform_, viewProjection);
}
