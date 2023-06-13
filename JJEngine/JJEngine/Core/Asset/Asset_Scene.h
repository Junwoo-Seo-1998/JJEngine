#pragma once
#include "Asset.h"
class Scene;
class Asset_Scene :public Asset {

	bool CheckIsDataLoaded() const override;
	bool LoadData()override;
	void UnloadData()override;

public:
	std::shared_ptr<Scene> data{ nullptr };

	AssetType GetAssetType()const override { return AssetType::Scene; }
};