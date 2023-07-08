#pragma once
#include "Asset.h"

class Mesh;
class Asset_Mesh :public Asset {

	bool CheckIsDataLoaded() const override;
	bool LoadData(std::filesystem::path p)override;
	void UnloadData()override;

public:
	std::shared_ptr<Mesh> data{ nullptr };

	AssetType GetAssetType()const override { return AssetType::Mesh; }
};