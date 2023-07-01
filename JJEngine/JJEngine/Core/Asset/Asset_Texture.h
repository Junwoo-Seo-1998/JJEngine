#pragma once
#include "Asset.h"

class Texture;
class Asset_Texture :public Asset {

	bool CheckIsDataLoaded() const override;
	bool LoadData(std::filesystem::path p)override;
	void UnloadData()override;

public:
	std::shared_ptr<Texture> data{nullptr};

	AssetType GetAssetType()const override { return AssetType::Texture; }
};