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
class TutorialManager;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
public: //サブクラス
	enum SceneName {
		Title,
		Tutorial,
		Main,
		GameClear,
		GameOver,
	};

	// チュートリアル段階
	enum TutorialSteps {
		Move,
		NormalShot,
		Orbs,
		FireBullet,
		IceBullet,
		ThunderBullet,
		OrbReinforcement,
		ChangeOrb,
		TutorialEnd
	};

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

private: // 更新処理

	/// <summary>
	/// タイトル
	/// </summary>
	void TitleUpdate();

	/// <summary>
	/// チュートリアル
	/// </summary>
	void TutorialUpdate();

	/// <summary>
	/// 本編
	/// </summary>
	void MainUpdate();

	/// <summary>
	/// クリア
	/// </summary>
	void GameClearUpdate();

	/// <summary>
	/// ゲームオーバー
	/// </summary>
	void GameOverUpdate();

	/// <summary>
	/// フェードインアウト
	/// </summary>
	void FadeInOutUpdate();

private: // セットアップ

	/// <summary>
	/// タイトル
	/// </summary>
	void TitleSetup();

	/// <summary>
	/// チュートリアル
	/// </summary>
	void TutorialSetup();

	/// <summary>
	/// 本編
	/// </summary>
	void MainSetup();

	/// <summary>
	/// クリア
	/// </summary>
	void GameClearSetup();

	/// <summary>
	/// ゲームオーバー
	/// </summary>
	void GameOverSetup();

	/// <summary>
	/// フェードインアウトセットアップ
	/// </summary>
	/// <param name="nextScene"></param>
	void FadeInOutSetUp(SceneName nextScene);

private: //描画

	/// <summary>
	/// タイトル
	/// </summary>
	void TitleDraw(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// チュートリアル
	/// </summary>
	void TutorialDraw(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 本編
	/// </summary>
	void MainDraw(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// クリア
	/// </summary>
	void GameClearDraw(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// ゲームオーバー
	/// </summary>
	void GameOverDraw(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// フェードインアウト
	/// </summary>
	void FadeInOutDraw();

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
	// エネミー死亡エフェクトモデル
	std::unique_ptr<Model> modelEnemyDeathEffect_;

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
	uint32_t texturehandleButton_RT_N_ = 0u; // RTトリガー
	uint32_t texturehandleButton_RT_P_ = 0u; // RTトリガー押下時
	uint32_t texturehandleButton_LT_N_ = 0u; // LTトリガー
	uint32_t texturehandleButton_LT_P_ = 0u; // LTトリガー押下時

	// レティクル用テクスチャ
	uint32_t textureHandleReticle_ = 0u;
	// 体力用テクスチャ
	uint32_t textureHandleHeart_ = 0u;
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
	
	// コントローラー非接続テクスチャ
	uint32_t textureHandleDisconectController_ = 0u;
	// コントローラー非接続スプライト
	std::unique_ptr<Sprite> disconectControllerUI_;

	// 効果音
	uint32_t soundHandleFootStep_[2];            // 歩行音
	uint32_t soundHandleJump_;                   // ジャンプ音
	uint32_t soundHandleLanding_;                // 着地音
	uint32_t soundHandlePlayerDamage_;           // ダメージ音
	uint32_t soundHandleShot_;                   // 通常射撃音
	uint32_t soundHandleAddOrb_;                 // オーブ追加音
	uint32_t soundHandleSpecialShot_;            // 特殊射撃音
	uint32_t soundHandleFireShot_;               // 炎弾着弾音
	uint32_t soundHandleDeployIceWall_;          // 氷弾展開音
	uint32_t soundHandleDamageIceWall_;          // 氷弾ダメージ音
	uint32_t soundHandleDestroyIceWall_;         // 氷弾破壊音
	uint32_t soundHandleDeployStartThunderArea_; // 雷エリア展開開始音
	uint32_t soundHandleDeployThunderArea_;      // 雷エリア展開音
	uint32_t soundHandleDeployEndThunderArea_;   // 雷エリア展開終了音
	uint32_t soundHandleDeployOrbChoice_;        // オーブ選択音
	uint32_t soundHandleDeployChangeOrbSuccess_; // オーブ変換成功音
	uint32_t soundHandleDeployChangeOrbFail_;    // オーブ変換失敗音
	uint32_t soundHandleDeployChoiceFireBullet_; // 炎弾選択音
	uint32_t soundHandleDeployChoiceIceBullet_; // 氷弾選択音

	//スプライト
	// ボスHP
	std::unique_ptr<Sprite> bossHpSprite_;
	uint32_t bossHpTextureHandle_ = 0u;

	// ボスHPフレーム
	std::unique_ptr<Sprite> bossHpFrameSprite_;
	uint32_t bossHpFrameTextureHandle_ = 0u;

	// エフェクトマネージャー
	EffectManager* effectManager_ = nullptr;
	// チュートリアルマネージャー
	TutorialManager* tutorialManager_ = nullptr;

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

	//現在のシーン
	SceneName currentScene_;
	//次のシーン
	SceneName nextScene_;

	// フェードインしているか
	bool isFadeIn_ = false;
	// フェードアウトしているか
	bool isFadeOut_ = false;
	// フェード時間
	const uint32_t kFadeTime_ = 60;
	// フェードタイマー
	uint32_t fadeTimer_ = 0;

	// フェードスプライト
	std::unique_ptr<Sprite> fadeSprite_;
	// フェードテクスチャ
	uint32_t fadeTextureHandle_ = 0u;
	// フェードポジション
	Vector2 fadePosition_;
	// フェード色
	Vector4 fadeColor_;
	// フェードサイズ
	Vector2 fadeSize_;

	// チュートリアルの段階
	int tutorialSteps_;

	//ゲーム終了したか
	bool theGameIsOver;

	// ゲームパッドの状態取得
	XINPUT_STATE joyState;
	XINPUT_STATE preJoyState;

};
