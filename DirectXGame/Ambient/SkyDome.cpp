#include "SkyDome.h"
#include <cassert>

void SkyDome::Intialize(Model* model) {
	// NULLチェック
	assert(model);

	// 引数の値をメンバ変数に代入
	model_ = model;

	// ワールド座標初期化
	worldTransform_.Initialize();
}

void SkyDome::Update() {
	// 行列転送
	worldTransform_.TransferMatrix();
}

void SkyDome::Draw(const ViewProjection& viewProjection) { 
	// モデル描画
	model_->Draw(worldTransform_, viewProjection); 
}
