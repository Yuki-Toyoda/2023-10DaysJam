#include "WorldTransform.h"
#include "math/MyMath.h"

void WorldTransform::UpdateMatrix() {

	// スケール、回転、平行移動を合成して行列を計算する
	matWorld_ = MyMath::MakeAffineMatrix(scale_, rotation_, translation_);

	// 親があれば親のワールド行列を掛ける
	if (parent_) {
		matWorld_ = matWorld_ * parent_->matWorld_;
	}

	// 行列転送
	TransferMatrix();
}