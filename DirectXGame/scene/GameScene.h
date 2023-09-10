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
#include "Collision/CollisionManager.h"
#include "Ambient/Field.h"

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

	//フィールド
	Field* field_ = nullptr;

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
	// エネミーモデル
	std::unique_ptr<Model> modelEnemy_;
	// ボスエネミーモデル
	std::unique_ptr<Model> modelBossEnemy_;
	// エネミーバレットモデル
	std::unique_ptr<Model> modelEnemyBullet_;

	// 破片モデル
	std::unique_ptr<Model> modelDebris_;
	// 氷破片モデル
	std::unique_ptr<Model> modelIceDebris_;
	// 火花モデル
	std::unique_ptr<Model> modelSpark_;

	// 1x1テクスチャ
	uint32_t textureHandle1x1_ = 0u;

	// 番号テクスチャ
	uint32_t textureHandleNumbers_[10];

	// ボタン用テクスチャ達
	uint32_t texturehandleDpad_ = 0u; // 十字ボタン
	uint32_t texturehandleDpadArrow_N_ = 0u; // 十字ボタン矢印
	uint32_t texturehandleDpadArrow_P_ = 0u; // 十字ボタン矢印押下時
	uint32_t texturehandleButton_X_N_ = 0u; // Xボタン
	uint32_t texturehandleButton_X_P_ = 0u; // Xボタン押下時
	uint32_t texturehandleButton_RT_N_ = 0u; // RTトリガー
	uint32_t texturehandleButton_RT_P_ = 0u; // RTトリガー押下時
	uint32_t texturehandleButton_RB_N_ = 0u; // RBトリガー
	uint32_t texturehandleButton_RB_P_ = 0u; // RBトリガー押下時
	uint32_t texturehandleButton_LT_N_ = 0u; // LTトリガー
	uint32_t texturehandleButton_LT_P_ = 0u; // LTトリガー押下時

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
	uint32_t texturehandleNone_ = 0u; // 撃てない
	// 選択テクスチャ
	uint32_t textureHandleSelectArrow_L_ = 0u;
	uint32_t textureHandleSelectArrow_R_ = 0u;
	// RBホールドテキストテクスチャ
	uint32_t textureHandleRBHoldText_ = 0u;
	// オーブ変換テキストテクスチャ
	uint32_t textureHandleChangeOrbText_ = 0u;
	uint32_t textureHandleChangeOrbText2_ = 0u;
	// オーブ変換に必要な敵数テキストテクスチャ
	uint32_t textureHandleNeedEnemyCountText_ = 0u;

	// 通常射撃テキストテクスチャ
	uint32_t textureHandleNormalShotText_ = 0u;
	// 特殊射撃テキストテクスチャ
	uint32_t textureHandleSpecialShotText_ = 0u;
	// 倍率テクスチャ
	uint32_t textureHandleMagnification[3] = {0u};

	// 氷壁用テクスチャ
	uint32_t textureHandleIceWallTex[3] = {0u};

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

	// エネミーマネージャー
	EnemyManager* enemyManager_;

	// 衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager;
};
