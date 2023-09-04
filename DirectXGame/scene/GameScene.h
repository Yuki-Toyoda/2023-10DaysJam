#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "Camera/Camera.h"
#include "Ambient/SkyDome.h"
#include "Ambient/Ground.h"
#include "Charactor/CharactorList.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	// ビュープロジェクション
	ViewProjection* viewProjection_;

	// 天球モデル
	std::unique_ptr<Model> modelSkyDome_;
	// 地面モデル
	std::unique_ptr<Model> modelGround_;

	// デバックカメラ
	std::unique_ptr<DebugCamera> debugCamera_;
	// デバックカメラ有効非有効
	bool enableDebugCamera_;

	// カメラクラス
	std::unique_ptr<Camera> camera_;

	// 天球クラス
	std::unique_ptr<SkyDome> skyDome_;
	// 地面クラス
	std::unique_ptr<Ground> ground_;

	// プレイヤークラス
	std::unique_ptr<Player> player_ = nullptr;

};
