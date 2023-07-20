#pragma once
#include "Core/Asset/AssetType.h"
#include "Core/Asset/Asset.h"
#include "Core/Asset/Asset_Texture.h"
#include "Core/Asset/Asset_Scene.h"
#include "Core/Asset/Asset_Mesh.h"
#include "Core/Asset/Asset_HDRTexture.h"
#include <memory>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include "Core/Utils/Assert.h"

class Asset;
struct Metadata;

class AssetManager
{
	std::unordered_map<AssetHandle, std::shared_ptr<Asset>> assets{};
	std::unordered_map<AssetHandle, std::shared_ptr<Metadata>> assetMetadatas{};
 	//std::unordered_map<int, AssetHandle> meshManager{};

	std::unordered_map<AssetHandle, std::shared_ptr<Asset>>::iterator latestFoundAsset;


	void GenAsset(std::shared_ptr<Asset>& empty_asset, AssetType type);
	std::shared_ptr<Asset> AddAsset(AssetType type, AssetHandle hd, std::filesystem::path path, bool IsMemoryOnly);
public:
	AssetManager() = default;
	virtual ~AssetManager() = default;


	bool ReadAData();
	void SaveAData();
	void UpdateAData();

	AssetHandle AddMemoryOnlyAsset(AssetType type);
	bool IsMemoryOnlyAsset(AssetHandle handle);

	std::shared_ptr<Metadata> GetMetadata(AssetHandle assetHandle);
	AssetType GetAssetType(AssetHandle assetHandle);
	std::shared_ptr<Asset> GetAsset(AssetHandle assetHandle);

	template<typename t>
	std::shared_ptr<t> GetCastedAsset(AssetHandle assetHandle);
	//template<typename t>
	//std::shared_ptr<t> GetAssetData(AssetHandle assetHandle);

	std::map < AssetHandle, std::string> projectScenes{};
	std::shared_ptr<Asset_Scene> GetEnrolledScene(std::string name);

	bool LoadData(AssetHandle assetHandle);
	bool ReloadData(AssetHandle assetHandle);
	bool UnloadData(AssetHandle assetHandle);
	bool IsAssetHandleValid(AssetHandle assetHandle);
	bool IsAssetLoaded(AssetHandle handle);

	AssetHandle GetHandleFromPath(std::filesystem::path p);
	std::shared_ptr<Asset> GetAssetFromPath(std::filesystem::path p);

	std::unordered_set<AssetHandle> GetAllMemoryOnlyAssetHandles();
	std::unordered_set<AssetHandle> GetAllFileAssetHandles();
	std::unordered_set<AssetHandle> GetAllAssetsWithType(AssetType type);
	const std::unordered_map<AssetHandle, std::shared_ptr<Asset>>& GetAllAssets();
	std::unordered_set<AssetHandle> GetAllHandles();
	const std::unordered_map<AssetHandle, std::shared_ptr<Metadata>>& GetAllMetadatas();
};

template<typename t>
inline std::shared_ptr<t> AssetManager::GetCastedAsset(AssetHandle assetHandle)
{
	ENGINE_ASSERT(false, "Not a appropriate type for asset");
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
	ENGINE_ASSERT(asset->GetAssetType() == AssetType::Texture, "Not a appropriate type for asset");
	return std::dynamic_pointer_cast<Asset_Texture, Asset>(asset);
}

template<>
inline std::shared_ptr<Asset_Scene> AssetManager::GetCastedAsset(AssetHandle assetHandle)
{
	std::shared_ptr<Asset> asset = GetAsset(assetHandle);
	ENGINE_ASSERT(asset->GetAssetType() == AssetType::Scene, "Not a appropriate type for asset");
	return std::dynamic_pointer_cast<Asset_Scene, Asset>(asset);
}

template<>
inline std::shared_ptr<Asset_Mesh> AssetManager::GetCastedAsset(AssetHandle assetHandle)
{
	std::shared_ptr<Asset> asset = GetAsset(assetHandle);
	ENGINE_ASSERT(asset->GetAssetType() == AssetType::Mesh, "Not a appropriate type for asset");
	return std::dynamic_pointer_cast<Asset_Mesh, Asset>(asset);
}

template<>
inline std::shared_ptr<Asset_HDRTexture> AssetManager::GetCastedAsset(AssetHandle assetHandle)
{
	std::shared_ptr<Asset> asset = GetAsset(assetHandle);
	ENGINE_ASSERT(asset->GetAssetType() == AssetType::HDR_Texture, "Not a appropriate type for asset");
	return std::dynamic_pointer_cast<Asset_HDRTexture, Asset>(asset);
}