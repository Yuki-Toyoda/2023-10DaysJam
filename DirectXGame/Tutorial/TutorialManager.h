#pragma once
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "Charactor/Player/Player.h"

// クラスの前方宣言
class EnemyManager;

/// <summary>
/// チュートリアルマネージャー
/// </summary>
class TutorialManager {
private: // サブクラス

	// チュートリアル段階
	enum TutorialSteps {
		Move, // 移動
		NormalShot, // 通常射撃
		Orbs, // オーブ
		FireBullet, // 炎弾
		IceBullet, // 氷弾
		ThunderBullet, // 雷弾
		OrbReinforcement, // オーブ許強化
		ChangeOrb, // オーブ変換
		TutorialEnd // チュートリアル終了
	};

	// チュートリアル演出用中間地点
	enum TutorialStagingWayPoint {
		WayPoint1,
		WayPoint2,
		WayPoint3,
		WayPoint4,
		WayPoint5,
	};

private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TutorialManager();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~TutorialManager();
public: // メンバ関数

	// コピーコンストラクタ無効
	TutorialManager(const TutorialManager& obj) = delete;
	// 代入演算子無効
	TutorialManager& operator=(const TutorialManager& obj) = delete;

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>クラスのインスタンス</returns>
	static TutorialManager* GetInstance();

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="modelsPlayer">チュートリアル用モデル</param>
	/// <param name="textureHandles">テクスチャ達</param>
	/// <param name="audioHandles">効果音達</param>
	void Initialize(
	    const std::vector<Model*>& tutorialModels,
	    const std::vector<uint32_t>& textureHandles, const std::vector<uint32_t>& audioHandles);;

	/// <summary>
	/// セットアップ関数
	/// </summary>
	void SetUp();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// スプライト描画
	/// </summary>
	void SpriteDraw();

public: // アクセッサ等

	/// <summary>
	/// チュートリアル終了状態のゲッター
	/// </summary>
	/// <returns>チュートリアル終了状態</returns>
	bool GetTutorialEnd() { return isEndTutorial; }

	/// <summary>
	///	プレイヤーのセッター
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void SetPlayer(Player* player) {
		player_ = player;
	}

private: // その他関数

	/// <summary>
	/// 移動チュートリアル関数
	/// </summary>
	void MoveTutorial();

	/// <summary>
	/// 通常射撃チュートリアル関数
	/// </summary>
	void NormalShotTutorial();

	/// <summary>
	/// オーブのチュートリアル
	/// </summary>
	void OrbTutorial();

	/// <summary>
	/// 炎弾チュートリアル
	/// </summary>
	void FireBulletTutorial();

	/// <summary>
	/// 氷弾チュートリアル
	/// </summary>
	void IceBulletTutorial();

	/// <summary>
	/// 雷弾チュートリアル
	/// </summary>
	void ThunderBulletTutorial();

	/// <summary>
	/// オーブ強化チュートリアル
	/// </summary>
	void OrbReinforcementTutorial();

	/// <summary>
	/// オーブ変換チュートリアル
	/// </summary>
	void ChangeOrbTutorial();

private: // メンバ変数

	// 入力検知用
	Input* input_ = nullptr;
	// ゲームパッドの状態取得
	XINPUT_STATE joyState;
	XINPUT_STATE preJoyState;

	// プレイヤー
	Player* player_;
	// 敵マネージャー
	EnemyManager* enemyManager_;

	// モデルリスト
	std::vector<Model*> models_;
	// テクスチャリスト
	std::vector<uint32_t> textureHandles_;
	// 効果音リスト
	std::vector<uint32_t> audioHandles_;

	// チュートリアル段階
	int tutorialSteps_;
	// 個々のチュートリアル演出中間地点
	int tutorialStagingWayPoint_;
	// チュートリアル演出用t
	float tutorialT_;

	// チュートリアル終了トリガー
	bool isEndTutorial;

	// チュートリアル画像表示トリガー
	bool displayTutorialImage_;

	// チュートリアル進捗
	float tutorialGageProgress_;
	// チュートリアルゲージ最大値
	const float kMaxTutorialGage_ = 100.0f;
	// 次のチュートリアルへ
	bool isNextTutorial_;

	// チュートリアル画像の表示切り替えのトリガー
	bool tutorialImageChangeTrigger_;
	int tutorialImagewayPoint_;

	// 敵のスポーン間隔
	float enemySpawnInterval_;

	// チュートリアル画像テクスチャ
	uint32_t textureHandleTutorialImage_;
	// チュートリアルテキストテクスチャ
	uint32_t textureHandleTutorialText_;
	// 次のチュートリアルへ促すテクスチャ
	uint32_t textureHandleNextTutorial_;

	// チュートリアルゲージの座標
	Vector2 tutorialGagePosition_;
	// チュートリアルゲージの大きさ
	Vector2 tutorialGageSize_;
	// チュートリアル進捗ゲージ背景用スプライト
	std::unique_ptr<Sprite> spriteTutorialGageBackGround_;
	// チュートリアル進捗ゲージ用スプライト
	std::unique_ptr<Sprite> spriteTutorialGage_;

	// チュートリアル画像用スプライト
	std::unique_ptr<Sprite> spriteTutorialImage_;
	// 次のチュートリアルへ促すスプライト
	std::unique_ptr<Sprite> spriteNextTutorial_;

	// チュートリアルテキストの座標
	Vector2 tutorialTextPosition_;
	// チュートリアルのゲージ座標
	Vector2 tutorialTextSize_;
	// チュートリアルテキスト背景用スプライト
	std::unique_ptr<Sprite> spriteTutorialTextBackGround_;
	// チュートリアルテキスト用スプライト
	std::unique_ptr<Sprite> spriteTutorialText_;

};
