#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

/// <summary>
/// 地面描画クラス
/// </summary>
class Ground {
public: // メンバ関数
	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="model">描画するモデル</param>
	void Intialize(Model* model);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

private: // メンバ変数
	// ワールド座標
	WorldTransform worldTransform_;

	// 3Dモデル
	Model* model_;
};
