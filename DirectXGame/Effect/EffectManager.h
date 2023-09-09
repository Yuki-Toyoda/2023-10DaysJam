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

public: // アクセッサ等

	/// <summary>
	///	ビルボード時のビュープロジェクションのセッター
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

public: // エフェクトの再生関数

	/// <summary>
	/// 爆発エフェクト再生関数
	/// </summary>
	/// <param name="models">破片モデル</param>
	/// <param name="translation">初期座標</param>
	/// <param name="strength">爆発強さ</param>
	void PlayExplosiveEffect(std::vector<Model*> models, Vector3 translation, float strength);

	/// <summary>
	/// 火花エフェクト再生関数
	/// </summary>
	/// <param name="models">描画モデル</param>
	/// <param name="translation">初期座標</param>
	/// <param name="time">継続時間</param>
	/// <param name="occurrenceRange">発生範囲</param>
	void PlaySparkEffect(
	    std::vector<Model*> models, Vector3 translation, float time, Vector2 occurrenceRange);

	/// <summary>
	/// 破片エフェクト再生関数
	/// </summary>
	/// <param name="models">描画モデル</param>
	/// <param name="translate">初期座標</param>
	/// <param name="occurrenceTranslate">発生位置ポインタ</param>
	/// <param name="occurrenceRotation">発生角度</param>
	/// <param name="time">継続時間</param>
	/// <param name="occurrenceRange">発生範囲</param>
	void PlayDebrisEffect(
	    std::vector<Model*> models, Vector3 translation, const Vector3* occurrenceTranslate,
	    const Vector3* occurrenceRotation, float time, Vector2 occurrenceRange);

private: // メンバ変数

	// エフェクトリスト
	std::list<BaseEffect*> effets_;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

};
