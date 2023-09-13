#pragma once

#include <array>
#include <cstdint>
#include <mutex>
#include <set>
#include <string>
#include <unordered_map>
#include <wrl.h>
#include <xaudio2.h>

/// <summary>
/// オーディオ
/// </summary>
class Audio {
public: // サブクラス

	// プレイヤーの効果音リスト
	enum playerAudios {
		FootStep1,         // 歩行音1
		FootStep2,         // 歩行音2
		Jump,			   // ジャンプ音
		Land,			   // 着地音
		Damage,			   // ダメージ音
		Shot,              // 通常射撃
		AddOrb,			   // オーブ追加音
		SpecialShot,       // 特殊射撃
		FireBulletLanding, // 炎弾着弾
		DeployIceWall, // 氷弾展開
		DamageIceWall, // 氷弾ダメージ
		DestroyIceWall, // 氷弾破壊
		DeployStartThunderArea, // 雷エリア展開開始音
		DeployThunderArea, // 雷エリア展開音
		DeployEndThunderArea, // 雷エリア展開終了音
		ChoiceOrb, // オーブ選択音
		ChangeOrbSuccess, // オーブ変換成功音
		ChangeOrbFail, // オーブ変換失敗音
		ChoiceFireBullet, // 炎弾選択音
		ChoiceIceBullet, // 氷弾選択音
	};

	// 弾の効果音リスト
	enum PlayerBulletAudios {
		FireBullet, // 炎弾着弾
		IceBulletDeploy, // 氷弾展開
		IceBulletDamage, // 氷弾ダメージ
		IceBulletDestroy, // 氷弾破壊
		ThunderBulletStart, // 雷エリア展開開始音
		ThunderBulletDeploy, // 雷エリア展開音
		ThunderBulletDeployEnd, // 雷エリア展開終了音
	};

	// チュートリアル効果音リスト
	enum TutorialAudios {
		NextTutorial,
		clearTutorial,
		endTutorial,
	};

	// オプション効果音リスト
	enum OptionAudios {
		MoveItem, // 項目選択音
		Open,
		ChangeValue,
		close,
	};

	// エネミー効果音リスト
	enum EnemyAudios {
		EnemyDamage,
		EnemyDeath
	};

	// ボスエネミー効果音リスト
	enum BossEnemyAudios { 
		BossEnemyDamage, 
		BossEnemyDeath
	};

public:
	// サウンドデータの最大数
	static const int kMaxSoundData = 256;

	// 音声データ
	struct SoundData {
		// 波形フォーマット
		WAVEFORMATEX wfex;
		// バッファ
		std::vector<uint8_t> buffer;
		// 名前
		std::string name_;
	};

	// 再生データ
	struct Voice {
		uint32_t handle = 0u;
		IXAudio2SourceVoice* sourceVoice = nullptr;
	};

	/// <summary>
	/// オーディオコールバック
	/// </summary>
	class XAudio2VoiceCallback : public IXAudio2VoiceCallback {
	public:
		// ボイス処理パスの開始時
		STDMETHOD_(void, OnVoiceProcessingPassStart)
		([[maybe_unused]] THIS_ UINT32 BytesRequired){};
		// ボイス処理パスの終了時
		STDMETHOD_(void, OnVoiceProcessingPassEnd)(THIS){};
		// バッファストリームの再生が終了した時
		STDMETHOD_(void, OnStreamEnd)(THIS){};
		// バッファの使用開始時
		STDMETHOD_(void, OnBufferStart)([[maybe_unused]] THIS_ void* pBufferContext){};
		// バッファの末尾に達した時
		STDMETHOD_(void, OnBufferEnd)(THIS_ void* pBufferContext);
		// 再生がループ位置に達した時
		STDMETHOD_(void, OnLoopEnd)([[maybe_unused]] THIS_ void* pBufferContext){};
		// ボイスの実行エラー時
		STDMETHOD_(void, OnVoiceError)
		([[maybe_unused]] THIS_ void* pBufferContext, [[maybe_unused]] HRESULT Error){};
	};

	static Audio* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::string& directoryPath = "Resources/");

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// WAV音声読み込み
	/// </summary>
	/// <param name="filename">WAVファイル名</param>
	/// <returns>サウンドデータハンドル</returns>
	uint32_t LoadWave(const std::string& filename);

	/// <summary>
	/// サウンドデータの解放
	/// </summary>
	/// <param name="soundData">サウンドデータ</param>
	void Unload(SoundData* soundData);

	/// <summary>
	/// 音声再生
	/// </summary>
	/// <param name="soundDataHandle">サウンドデータハンドル</param>
	/// <param name="loopFlag">ループ再生フラグ</param>
	/// <param name="volume">ボリューム
	/// 0で無音、1がデフォルト音量。あまり大きくしすぎると音割れする</param>
	/// <returns>再生ハンドル</returns>
	uint32_t PlayWave(uint32_t soundDataHandle, bool loopFlag = false, float volume = 1.0f);

	/// <summary>
	/// 音声停止
	/// </summary>
	/// <param name="voiceHandle">再生ハンドル</param>
	void StopWave(uint32_t voiceHandle);

	/// <summary>
	/// 音声再生中かどうか
	/// </summary>
	/// <param name="voiceHandle">再生ハンドル</param>
	/// <returns>音声再生中かどうか</returns>
	bool IsPlaying(uint32_t voiceHandle);

	/// <summary>
	/// 音声一時停止
	/// </summary>
	/// <param name="voiceHandle">再生ハンドル</param>
	void PauseWave(uint32_t voiceHandle);

	/// <summary>
	/// 音声一時停止からの再開
	/// </summary>
	/// <param name="voiceHandle">再生ハンドル</param>
	void ResumeWave(uint32_t voiceHandle);

	/// <summary>
	/// 音量設定
	/// </summary>
	/// <param name="voiceHandle">再生ハンドル</param>
	/// <param name="volume">ボリューム
	/// 0で無音、1がデフォルト音量。あまり大きくしすぎると音割れする</param>
	void SetVolume(uint32_t voiceHandle, float volume);

private:
	Audio() = default;
	~Audio() = default;
	Audio(const Audio&) = delete;
	const Audio& operator=(const Audio&) = delete;

	// XAudio2のインスタンス
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	// サウンドデータコンテナ
	std::array<SoundData, kMaxSoundData> soundDatas_;
	// 再生中データコンテナ
	// std::unordered_map<uint32_t, IXAudio2SourceVoice*> voices_;
	std::set<Voice*> voices_;
	// サウンド格納ディレクトリ
	std::string directoryPath_;
	// 次に使うサウンドデータの番号
	uint32_t indexSoundData_ = 0u;
	// 次に使う再生中データの番号
	uint32_t indexVoice_ = 0u;
	// オーディオコールバック
	XAudio2VoiceCallback voiceCallback_;
	std::mutex voiceMutex_;
};
