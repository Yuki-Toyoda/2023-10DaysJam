#pragma once
#include <vector>
#include "Model.h"
#include "ViewProjection.h"
#include "EffectList.h"

/// <summary>
/// エフェクトマネージャー
/// </summary>
class EffectManager {
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EffectManager();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EffectManager();

public: // メンバ関数
	// コピーコンストラクタ無効
	EffectManager(const EffectManager& obj) = delete;
	// 代入演算子無効
	EffectManager& operator=(const EffectManager& obj) = delete;

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>クラスのインスタンス</returns>
	static EffectManager* GetInstance();

	/// <summary>
	/// リストをクリアする関数
	/// </summary>
	void Initialize();

	/// <summary>
	/// 全てのエフェクトの更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 全てのエフェクトの描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

public: // エフェクトの再生関数

	/// <summary>
	/// 爆発エフェクト再生関数
	/// </summary>
	/// <param name="models">破片モデル</param>
	/// <param name="translation">初期座標</param>
	/// <param name="strength">爆発強さ</param>
	void PlayExplosiveEffect(std::vector<Model*> models, Vector3 translation, float strength);

private: // メンバ変数

	// エフェクトリスト
	std::list<BaseEffect*> effets_;

};
