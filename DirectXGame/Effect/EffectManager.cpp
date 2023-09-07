#include "EffectManager.h"

EffectManager::EffectManager() {}

EffectManager::~EffectManager() {}

EffectManager* EffectManager::GetInstance() { 
	// 関数内Static変数として宣言
	static EffectManager instance;
	return &instance;
}

void EffectManager::Initialize() { 
	// リストのクリア
	effets_.clear(); 
}

void EffectManager::Update() {
	// 終了フラグの立ったエフェクトを削除
	effets_.remove_if([](BaseEffect* effect) {
		if (effect->GetIsEnd()) {
			delete effect;
			return true;
		}
		return false;
	});

	// 全てのエフェクトの更新処理
	for (BaseEffect* effect : effets_) {
		effect->Update();
	}
}

void EffectManager::Draw(const ViewProjection& viewProjection) {
	// 全てのエフェクトの描画処理
	for (BaseEffect* effect : effets_) {
		effect->Draw(viewProjection);
	}
}

void EffectManager::PlayExplosiveEffect(std::vector<Model*> models, Vector3 translation) {
	// 新しいエフェクトのインスタンスを生成
	ExplosiveEffect* explosiveEffect = new ExplosiveEffect();
	// 生成したエフェクトの初期化
	explosiveEffect->initialize(models, translation);
	// 生成したエフェクトをリストに追加
	effets_.push_back(explosiveEffect);
}
