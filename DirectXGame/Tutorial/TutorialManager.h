#pragma once
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "Charactor/Player/Player.h"

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
		OrbReinforcement,
		ChangeOrb,
		TutorialEnd
	};

public: // メンバ関数

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
	bool GetTutorialEnd() { return isTutorialEnd_; }

private: // メンバ関数

	// モデルリスト
	std::vector<Model*> models_;
	// テクスチャリスト
	std::vector<uint32_t> textureHandles_;
	// 効果音リスト
	std::vector<uint32_t> audioHandles_;

	// チュートリアル段階
	int tutorialSteps_;

	// チュートリアル終了トリガー
	bool isTutorialEnd_;

	// チュートリアル画像表示トリガー
	bool displayTutorialImage_;

	// チュートリアル画像テクスチャ
	uint32_t textureHandleTutorialImage_;
	// チュートリアルテキストテクスチャ
	uint32_t textureHandleTutorialText_;

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

	// チュートリアルテキストの座標
	Vector2 tutorialTextPosition_;
	// チュートリアルのゲージ座標
	Vector2 tutorialTextSize_;
	// チュートリアルテキスト背景用スプライト
	std::unique_ptr<Sprite> spriteTutorialTextBackGround_;
	// チュートリアルテキスト用スプライト
	std::unique_ptr<Sprite> spriteTutorialText_;

};
