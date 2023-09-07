#include "ExplosiveEffect.h"

void ExplosiveEffect::initialize(std::vector<Model*> models, Vector3 translate) {
	// 基底クラスの初期化
	BaseEffect::initialize(models, translate);

}

void ExplosiveEffect::Update() { 

	// 基底クラスの更新
	BaseEffect::Update(); 
}

void ExplosiveEffect::Draw(const ViewProjection& viewProjection) { 
	models_[0]->Draw(worldTransform_, viewProjection);
}
