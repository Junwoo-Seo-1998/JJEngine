#pragma once
#include "Asset.h"

class Texture;
class Asset_HDRTexture :public Asset {
	bool CheckIsDataLoaded() const override;
	void UnloadData()override;
	bool LoadData(std::filesystem::path p)override;

public:
	std::shared_ptr<Texture> data{ nullptr };

	AssetType GetAssetType()const override { return AssetType::HDR_Texture; }
};