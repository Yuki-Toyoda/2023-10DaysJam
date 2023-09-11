#pragma once

#include <array>
#include <d3dx12.h>
#include <string>
#include <unordered_map>
#include <wrl.h>

/// <summary>
/// テクスチャマネージャ
/// </summary>
class TextureManager {
public: // サブクラス

	enum PlayerTextures {
		// サンプルテクスチャ
		Texture1x1, // 1x1

		// 番号
		texture_0,
		texture_1,
		texture_2,
		texture_3,
		texture_4,
		texture_5,
		texture_6,
		texture_7,
		texture_8,
		texture_9,

		// ボタン用テクスチャ
		Dpad,        // 十字ボタン
		RT_N,        // RTトリガー
		RT_P,        // RTトリガー押下時
		LT_N,        // LTトリガー
		LT_P,        // LTトリガー押下時

		// UIテクスチャ
		Reticle, // 照準
		Heart, // ハート
		Orb, // オーブ
		FireBullet, // 炎弾
		IceBullet, // 氷弾
		ThunderBullet, // 雷弾
		None, // 撃てない
		selectOrb, // オーブ選択
		selectArrow_L, // オーブ選択左矢印
		selectArrow_R, // オーブ選択右矢印
		changeOrbText, // 選択オーブテキスト
		changeOrbText2, // 選択オーブテキスト2
		NeedEnemyCountText, // 変換に必要な敵数テキスト
		NormalShotText, // 通常射撃テキスト
		SpecialShotText, // 特殊射撃テキスト
		Texturex1, // x1テクスチャ
		Texturex2, // x1テクスチャ
		Texturex3, // x1テクスチャ
		

		// エフェクト用テクスチャ
		IceWall, // 氷壁1段階目
		IceWallDamage1, // 氷壁2段階目
		IceWallDamage2, // 氷壁3段階目

	};

public:
	// デスクリプターの数
	static const size_t kNumDescriptors = 256;

	/// <summary>
	/// テクスチャ
	/// </summary>
	struct Texture {
		// テクスチャリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		// シェーダリソースビューのハンドル(CPU)
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
		// シェーダリソースビューのハンドル(CPU)
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
		// 名前
		std::string name;
	};

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <returns>テクスチャハンドル</returns>
	static uint32_t Load(const std::string& fileName);

	/// <summary>
	/// 読み込み解除
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	static bool Unload(uint32_t textureHandle);

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// システム初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	void Initialize(ID3D12Device* device, std::string directoryPath = "Resources/");

	/// <summary>
	/// 全テクスチャリセット
	/// </summary>
	void ResetAll();

	/// <summary>
	/// リソース情報取得
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <returns>リソース情報</returns>
	const D3D12_RESOURCE_DESC GetResoureDesc(uint32_t textureHandle);

	/// <summary>
	/// デスクリプタテーブルをセット
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="rootParamIndex">ルートパラメータ番号</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void SetGraphicsRootDescriptorTable(
	    ID3D12GraphicsCommandList* commandList, UINT rootParamIndex, uint32_t textureHandle);

private:
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	// C++のbitsetが内部詳細にアクセスできないのでfindFirst用に自作
	template<size_t kNumberOfBits> class Bitset {

	public:
		Bitset();
		size_t FindFirst() const;
		void Set(size_t bitIndex, bool value = true);
		void Reset();
		void Reset(size_t bitIndex);
		bool Test(size_t bitIndex) const;

	private:
		uint64_t& GetWord(size_t bitIndex);

	private:
		static constexpr size_t kCountOfWord =
		    (kNumberOfBits == 0 ? 1 : ((kNumberOfBits - 1) / (8 * sizeof(uint64_t)) + 1));
		static constexpr size_t kBitsPerWord = 8 * sizeof(uint64_t);
		static constexpr size_t kBitsPerWordMask = kBitsPerWord - 1;
		static constexpr size_t kBitIndexToWordIndex = 6;

		uint64_t words_[kCountOfWord];
	};

	// デバイス
	ID3D12Device* device_;
	// デスクリプタサイズ
	UINT sDescriptorHandleIncrementSize_ = 0u;
	// ディレクトリパス
	std::string directoryPath_;
	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	// テクスチャコンテナ
	std::array<Texture, kNumDescriptors> textures_;
	Bitset<kNumDescriptors> useTable_;

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	uint32_t LoadInternal(const std::string& fileName);

	/// <summary>
	/// 読み込み解除
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	bool UnloadInternal(uint32_t textureHandle);
};
