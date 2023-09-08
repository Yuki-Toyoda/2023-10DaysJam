#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

/// <summary>
/// 全てのエフェクトの基底クラス
/// </summary>
class BaseEffect {
public: // メンバ関数
	
	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="models">モデル</param>
	/// <param name="translate">開始座標</param>
	virtual void initialize(std::vector<Model*> models, Vector3 translate);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="viewProjection"><ビュープロジェクション/param>
	virtual void Draw(const ViewProjection& viewProjection);

public: // アクセッサ等

	/// <summary>
	/// 終了状態セッター
	/// </summary>
	/// <param name="isEnd">終了状態</param>
	void SetIsEnd(bool isEnd) { isEnd_ = isEnd; }
	/// <summary>
	/// 終了状態ゲッター
	/// </summary>
	/// <returns>終了状態</returns>
	bool GetIsEnd() { return isEnd_; }

protected: // メンバ変数

	// エフェクトの発生座標
	WorldTransform worldTransform_;

	// エフェクトに使用するモデル
	std::vector<Model*> models_;

	// エフェクトの終了状態
	bool isEnd_;
};
