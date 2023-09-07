#include "ExplosiveEffect.h"

void ExplosiveEffect::initialize(std::vector<Model*> models, Vector3 translate) {
	// 基底クラスの初期化
	BaseEffect::initialize(models, translate);

	t_ = 0.0f;

}

void ExplosiveEffect::Update() { 

	if (t_ <= 3.0f)
		t_ += 1.0f / 60.0f;
	else
		isEnd_ = true;

	// 基底クラスの更新
	BaseEffect::Update(); 
}

void ExplosiveEffect::Draw(const ViewProjection& viewProjection) { 
	models_[0]->Draw(worldTransform_, viewProjection);
}
