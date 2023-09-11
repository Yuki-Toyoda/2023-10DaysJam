#include "EffectManager.h"

EffectManager::EffectManager() {}

EffectManager::~EffectManager() {
	// リスト削除
	// 終了フラグの立ったエフェクトを削除
	effets_.remove_if([](BaseEffect* effect) {
		delete effect;
		return true;
	});
}

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

void EffectManager::PlayExplosiveEffect(
    std::vector<Model*> models, Vector3 translation, float strength) {
	// 新しいエフェクトのインスタンスを生成
	ExplosiveEffect* newEffect = new ExplosiveEffect();
	// 生成したエフェクトの初期化
	newEffect->initialize(models, translation, strength);
	// 生成したエフェクトをリストに追加
	effets_.push_back(newEffect);
}

void EffectManager::PlaySparkEffect(
    std::vector<Model*> models, Vector3 translation, float time, Vector2 occurrenceRange) {
	// 新しいエフェクトのインスタンスを生成
	SparkEffect* newEffect = new SparkEffect();
	// 生成したエフェクトの初期化
	newEffect->initialize(models, translation, time, occurrenceRange, viewProjection_);
	// 生成したエフェクトをリストに追加
	effets_.push_back(newEffect);
}

void EffectManager::PlayDebrisEffect(
    std::vector<Model*> models, Vector3 translation, const Vector3* occurrenceTranslate,
    const Vector3* occurrenceRotation, float time,
    Vector2 occurrenceRange) {
	// 新しいエフェクトのインスタンスを生成
	DebrisEffect* newEffect = new DebrisEffect();
	// 生成したエフェクトの初期化
	newEffect->initialize(
	    models, translation, occurrenceTranslate, occurrenceRotation, time, occurrenceRange);
	// 生成したエフェクトをリストに追加
	effets_.push_back(newEffect);
}

void EffectManager::PlayEnemyDeathEffect(
    std::vector<Model*> models, uint32_t textureHandle, Vector3 translate) {

	// 新しいエフェクトのインスタンスを生成
	EnemyDeathEffect* newEffect = new EnemyDeathEffect();
	// 生成したエフェクトの初期化
	newEffect->initialize(models, textureHandle, translate);
	// 生成したエフェクトをリストに追加
	effets_.push_back(newEffect);

}
