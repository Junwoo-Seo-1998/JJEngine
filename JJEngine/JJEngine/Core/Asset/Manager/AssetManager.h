#pragma once
#include "Core/Asset/AssetType.h"
#include "Core/Asset/Asset.h"
#include "Core/Asset/Asset_Texture.h"
#include "Core/Asset/Asset_Scene.h"
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "Core/Utils/Assert.h"

class Asset;
struct Metadata;

class AssetManager
{
	std::unordered_map<AssetHandle, std::shared_ptr<Asset>> assets{};
	std::unordered_map<AssetHandle, std::shared_ptr<Metadata>> assetMetadatas{};

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
	std::shared_ptr<t> GetCastedAsset(AssetHandle assetHandle);
	//template<typename t>
	//std::shared_ptr<t> GetAssetData(AssetHandle assetHandle);

	// manual load func
	// manual unload func
	bool ReloadData(AssetHandle assetHandle);
	bool IsAssetHandleValid(AssetHandle assetHandle);
	bool IsAssetLoaded(AssetHandle handle);

	AssetHandle GetHandleFromPath(std::filesystem::path p);
	std::shared_ptr<Asset> GetAssetFromPath(std::filesystem::path p);
	std::unordered_set<AssetHandle> GetAllAssetsWithType(AssetType type);
	const std::unordered_map<AssetHandle, std::shared_ptr<Asset>>& GetAllAssets();
	std::unordered_set<AssetHandle> GetAllHandles();

};

template<typename t>
inline std::shared_ptr<t> AssetManager::GetCastedAsset(AssetHandle assetHandle)
{
	ASSERT("Not a appropriate type for asset");
	return std::shared_ptr<t>();
}

//template<typename t>
//inline std::shared_ptr<t> AssetManager::GetAssetData(AssetHandle assetHandle)
//{
//	ASSERT("Not a appropriate type for asset");
//	// type check
//	// casting data
//	return std::shared_ptr<t>();
//}


template<>
inline std::shared_ptr<Asset_Texture> AssetManager::GetCastedAsset(AssetHandle assetHandle)
{
	std::shared_ptr<Asset> asset = GetAsset(assetHandle);
	if (asset->GetAssetType() != AssetType::Texture) ASSERT("Not a appropriate type for asset");
	return std::dynamic_pointer_cast<Asset_Texture, Asset>(asset);
}

template<>
inline std::shared_ptr<Asset_Scene> AssetManager::GetCastedAsset(AssetHandle assetHandle)
{
	std::shared_ptr<Asset> asset = GetAsset(assetHandle);
	if (asset->GetAssetType() != AssetType::Scene) ASSERT("Not a appropriate type for asset");
	return std::dynamic_pointer_cast<Asset_Scene, Asset>(asset);
}