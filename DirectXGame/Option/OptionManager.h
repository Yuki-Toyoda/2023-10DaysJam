#pragma once
#include "Input.h"

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
	void Initialize(Camera* camera);
	
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


};
