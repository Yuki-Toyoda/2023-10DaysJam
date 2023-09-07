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

// クラスの前方宣言
class EffectManager;

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
	// 通常弾モデル
	std::unique_ptr<Model> modelBullet_;
	// 炎弾モデル
	std::unique_ptr<Model> modelFireBullet_;
	// 氷弾モデル
	std::unique_ptr<Model> modelIceBullet_;
	// 雷弾モデル
	std::unique_ptr<Model> modelThunderBullet_;

	// ボタン用テクスチャ達
	uint32_t texturehandleDpad_ = 0u; // 十字ボタン
	uint32_t texturehandleDpadArrow_N_ = 0u; // 十字ボタン矢印
	uint32_t texturehandleDpadArrow_P_ = 0u; // 十字ボタン矢印押下時

	// 1x1テクスチャ
	uint32_t textureHandle1x1_ = 0u;
	// レティクル用テクスチャ
	uint32_t textureHandleReticle_ = 0u;
	// オーブテクスチャ
	uint32_t texturehandleOrb_ = 0u;
	// 選択しているオーブを示すテクスチャ
	uint32_t textureHandleSelectedOrb_ = 0u;

	// 特殊弾テクスチャ
	uint32_t texturehandleFireBullet_ = 0u; // 炎弾
	uint32_t texturehandleIceBullet_ = 0u; // 炎弾
	uint32_t texturehandleThunderBullet_ = 0u; // 雷弾

	// 選択不可テクスチャ
	uint32_t textureHandleX_ = 0u;
	// 選択テクスチャ
	uint32_t textureHandleSelectArrow_L_ = 0u;
	uint32_t textureHandleSelectArrow_R_ = 0u;

	// エフェクトマネージャー
	EffectManager* effectManager_ = nullptr;

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
