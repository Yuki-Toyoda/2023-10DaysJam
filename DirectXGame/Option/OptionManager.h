#pragma once
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "audio/Audio.h"

// カメラの前方宣言
class Camera;

class OptionManager {

public: //サブクラス
	enum items {
		Fov, // 視野角
		CameraSensitivityX, // カメラ感度X
		CameraSensitivityY, // カメラ感度Y
	};

public: // 静的メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static OptionManager* GetInstance();

public: //メンバ関数
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="textureHandles">テクスチャリスト</param>
	/// <param name="audioHandles">効果音リスト</param>
	void Initialize(
	    Camera* camera, const std::vector<uint32_t>& textureHandles,
	    const std::vector<uint32_t>& audioHandles);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update(_XINPUT_GAMEPAD gamePad, _XINPUT_GAMEPAD preGamePad);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 開いたり閉じたり
	/// </summary>
	void OpenClose(_XINPUT_GAMEPAD gamePad, _XINPUT_GAMEPAD preGamePad);

public: // アクセッサ

	/// <summary>
	/// 開かれているかセッター
	/// </summary>
	/// <param name="isOpen"></param>
	void SetIsOpen(bool isOpen) { isOpen_ = isOpen; };

	/// <summary>
	/// 開かれているかゲッター
	/// </summary>
	/// <returns></returns>
	bool GetIsOpen() { return isOpen_; };

private: // シングルトンパターン
	OptionManager() = default;
	~OptionManager() = default;
	OptionManager(const OptionManager&) = delete;
	OptionManager& operator=(const OptionManager&) = delete;

private: // メンバ変数

	// 音
	Audio* audio_ = nullptr;

	// テクスチャリスト
	std::vector<uint32_t> textureHandles_;
	// 効果音リスト
	std::vector<uint32_t> audioHandles_;

	// 開かれているか
	bool isOpen_;

	//カメラ設定
	Camera* camera_ = nullptr;

	const float fovPulsValue_ = 0.01f;

	const float cameraSensitivityPulsValue_ = 0.001f;

	//現在の項目
	items itemNow;

	// クールタイム
	uint32_t besideButtonCooltimer_;

	const uint32_t kBesideButtonCooltime_ = 5;

	// 背景スプライト
	std::unique_ptr<Sprite> spriteBackGround_;

	// ゲージ長さ
	const Vector2 GageLength = {450.0f, 32.0f};

	// FOVスプライト座標
	Vector2 spriteFOVPosition_;
	// FOVスプライト
	std::unique_ptr<Sprite> spriteFOV_;
	// FOVゲージ背景
	std::unique_ptr<Sprite> spriteFOVGageBackGround_;
	// FOVゲージ
	std::unique_ptr<Sprite> spriteFOVGage_;

	// 水平感度スプライト座標
	Vector2 spriteCameraSensitivityXPosition_;
	// 水平感度スプライト
	std::unique_ptr<Sprite> spriteCameraSensitivityX_;
	// 水平感度ゲージ背景
	std::unique_ptr<Sprite> spriteCameraSensitivityXGageBackGround_;
	// 水平感度ゲージ
	std::unique_ptr<Sprite> spriteCameraSensitivityXGage_;

	// 垂直感度スプライト座標
	Vector2 spriteCameraSensitivityYPosition_;
	// 垂直感度スプライト
	std::unique_ptr<Sprite> spriteCameraSensitivityY_;
	// 垂直感度ゲージ背景
	std::unique_ptr<Sprite> spriteCameraSensitivityYGageBackGround_;
	// 垂直感度ゲージ
	std::unique_ptr<Sprite> spriteCameraSensitivityYGage_;

	// 選択項目スプライト座標
	Vector2 spriteSelectItemPosition_;
	// 選択項目スプライト
	std::unique_ptr<Sprite> spriteSelectItem_;

	Vector2 spriteNumberStartPos_;
	// 数値スプライト
	std::unique_ptr<Sprite> spriteNumbers_[3];

};
