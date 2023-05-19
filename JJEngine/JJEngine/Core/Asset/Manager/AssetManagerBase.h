#pragma once
#include "Core/Asset/AssetType.h"
#include "Core/Asset/Asset.h"
#include <memory>
#include <unordered_map>
#include <unordered_set>

class Asset;

class AssetManagerBase
{
public:
	AssetManagerBase() = default;
	virtual ~AssetManagerBase() = default;

	virtual AssetType GetAssetType(AssetHandle assetHandle) = 0;
	virtual std::shared_ptr<Asset> GetAsset(AssetHandle assetHandle) = 0;
	virtual void AddMemoryOnlyAsset(std::shared_ptr<Asset> asset) = 0;
	virtual bool ReloadData(AssetHandle assetHandle) = 0;
	virtual bool IsAssetHandleValid(AssetHandle assetHandle) = 0;
	virtual bool IsMemoryAsset(AssetHandle handle) = 0;
	virtual bool IsAssetLoaded(AssetHandle handle) = 0;

	virtual std::unordered_set<AssetHandle> GetAllAssetsWithType(AssetType type) = 0;
	virtual const std::unordered_map<AssetHandle, std::shared_ptr<Asset>>& GetLoadedAssets() = 0;
	virtual const std::unordered_map<AssetHandle, std::shared_ptr<Asset>>& GetMemoryOnlyAssets() = 0;
};
