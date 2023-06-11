#include "AssetManager.h"
#include "Core/Utils/YAML_IMPL.hpp"

#define ADATA_PATH "./Assets.AData"

#define SCN ".scn"
#define PNG ".png"
#define JPG ".jpg"

#define YM_ADATA "ADATA"
#define YM_ASSETS "Assets"
#define YM_ASSET_HANDLE "Handle"
#define YM_ASSET_TYPE "Type"
#define YM_ASSET_PATH "FilePath"

void AssetManager::AddAsset(std::shared_ptr<Asset>& empty_asset, AssetType type)
{
	switch (type)
	{
	case AssetType::Scene:break;
		empty_asset = std::make_shared<Asset>();
	case AssetType::Texture:break;
		empty_asset = std::make_shared<Asset>();
	default:
		ASSERT("Unexpected Asset type");
	}
}

bool AssetManager::ReadAData()
{
	std::ifstream file{ ADATA_PATH };
	std::stringstream strStream{};
	strStream << file.rdbuf();

	YAML::Node data = YAML::Load(strStream.str());
	if (!data[YM_ADATA]) return false;

	auto assetDatas = data[YM_ASSETS];
	for (auto D : assetDatas) {
		AssetHandle handle = D[YM_ASSET_HANDLE].as<AssetHandle>();
		AssetType type = AssetType(D[YM_ASSET_TYPE].as<int>());

		std::shared_ptr<Asset> temp;
		AddAsset(temp,type);
		assets[handle] = temp;
		temp->Handle = handle;
		temp->path = D[YM_ASSET_PATH].as<std::filesystem::path>();
	}
	return true;
}

void AssetManager::SaveAData()
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << YM_ADATA;
	out << YAML::Value << "All engine resorce assets";
	out << YAML::Key << YM_ASSETS;
	out << YAML::Value << YAML::BeginSeq;
	for (auto& a : assets) {
		out << YAML::BeginMap;
		YAML_KEY_VALUE(out, YM_ASSET_HANDLE, a.second->Handle);
		YAML_KEY_VALUE(out, YM_ASSET_TYPE, static_cast<int>(a.second->GetAssetType()));
		YAML_KEY_VALUE(out, YM_ASSET_PATH, a.second->path.c_str());
		out << YAML::EndMap;
	}
	out << YAML::EndSeq;

	std::ofstream file{ ADATA_PATH };
	file << out.c_str();
}

void AssetManager::UpdateAData()
{
	// check all assets in resorce folder
}

AssetType AssetManager::GetAssetType(AssetHandle assetHandle)
{
	if (IsAssetHandleValid(assetHandle) == false) return AssetType::None;
	return latestFoundAsset->second->GetAssetType();
}

std::shared_ptr<Asset> AssetManager::GetAsset(AssetHandle assetHandle)
{
	if (IsAssetHandleValid(assetHandle) == false) ASSERT("No asset is there");
	if (latestFoundAsset->second->CheckIsDataLoaded() == false) {
		if (latestFoundAsset->second->LoadData() == false) ASSERT("Fail to load asset");
	}
	return latestFoundAsset->second;
}

bool AssetManager::ReloadData(AssetHandle assetHandle)
{
	if (IsAssetHandleValid(assetHandle) == false) return false;
	latestFoundAsset->second->UnloadData();
	if (latestFoundAsset->second->LoadData() == false) return false;
	return true;
}

bool AssetManager::IsAssetHandleValid(AssetHandle assetHandle)
{
	latestFoundAsset = std::find(assets.begin(), assets.end(), assetHandle);
	if (latestFoundAsset == assets.end()) return false;
	return true;
}

bool AssetManager::IsAssetLoaded(AssetHandle handle)
{
	if (IsAssetHandleValid(handle) == false) return false;
	return latestFoundAsset->second->CheckIsDataLoaded();
}

std::unordered_set<AssetHandle> AssetManager::GetAllAssetsWithType(AssetType type)
{
	std::unordered_set<AssetHandle> handles{};
	for (auto& a:assets) {
		if (a.second->GetAssetType() == type) {
			handles.insert(a.second->Handle);
		}
	}
	return handles;
}

const std::unordered_map<AssetHandle, std::shared_ptr<Asset>>& AssetManager::GetAllAssets()
{
	return assets;
}

std::unordered_set<AssetHandle> AssetManager::GetAllHandles()
{
	std::unordered_set<AssetHandle> handles{};
	for (auto& a : assets) {
		handles.insert(a.second->Handle);
	}
	return handles;
}
