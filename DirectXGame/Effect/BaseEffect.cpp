#include "BaseEffect.h"

void BaseEffect::initialize(std::vector<Model*> models, Vector3 translate) { 
	// モデル受け取り
	models_ = models; 

	// 発生座標の初期化
	worldTransform_.Initialize();
	// 発生座標設定
	worldTransform_.translation_ = translate;

	// 終了していない状態に
	isEnd_ = false;
}

void BaseEffect::Update() { 
	// ワールド座標の更新
	worldTransform_.UpdateMatrix(); 
}

void BaseEffect::Draw(const ViewProjection& viewProjection) { 
	// 警告回避用
	viewProjection; 
}
