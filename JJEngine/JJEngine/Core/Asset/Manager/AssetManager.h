#pragma once
#include "Core/Asset/AssetType.h"
#include "Core/Asset/Asset.h"
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "Core/Utils/Assert.h"

class Asset;

class AssetManager
{
	std::unordered_map<AssetHandle, std::shared_ptr<Asset>> assets{};

	std::unordered_map<AssetHandle, std::shared_ptr<Asset>>::iterator latestFoundAsset;


	void AddAsset(std::shared_ptr<Asset>& empty_asset, AssetType type);

public:
	AssetManager() = default;
	virtual ~AssetManager() = default;

	bool ReadAData();
	void SaveAData();
	void UpdateAData();

	AssetType GetAssetType(AssetHandle assetHandle);
	std::shared_ptr<Asset> GetAsset(AssetHandle assetHandle);

	template<typename t>
	std::shared_ptr<t> GetAssetData(AssetHandle assetHandle);

	bool ReloadData(AssetHandle assetHandle);
	bool IsAssetHandleValid(AssetHandle assetHandle);
	bool IsAssetLoaded(AssetHandle handle);

	std::unordered_set<AssetHandle> GetAllAssetsWithType(AssetType type);
	const std::unordered_map<AssetHandle, std::shared_ptr<Asset>>& GetAllAssets();
	std::unordered_set<AssetHandle> GetAllHandles();

};

template<typename t>
inline std::shared_ptr<t> AssetManager::GetAssetData(AssetHandle assetHandle)
{
	ASSERT("Not a appropriate type for asset");
	// type check
	// casting data
	return std::shared_ptr<t>();
}


