#include "AssetManager.h"
#include "Core/Utils/YAML_IMPL.hpp"
#include "Core/Utils/File.h"
#include "Core/Utils/UUIDGenerator.h"

#define ADATA_PATH "./Assets.AData"
#define RESORCE_PATH "./Resources"

//Assets
#define SCN ".scn"
#define PNG ".png"
#define JPG ".jpg"

// YAML impl
#define YM_ADATA "ADATA"
#define YM_ASSETS "Assets"
#define YM_ASSET_HANDLE "Handle"
#define YM_ASSET_TYPE "Type"
#define YM_ASSET_PATH "FilePath"

void AssetManager::AddAsset(std::shared_ptr<Asset>& empty_asset, AssetType type)
{
	switch (type)
	{
	case AssetType::Scene:
		empty_asset = std::make_shared<Asset_Scene>();
		break;
	case AssetType::Texture:
		empty_asset = std::make_shared<Asset_Texture>();
		break;
	default:
		ENGINE_ASSERT(false,"Unexpected Asset type");
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
		temp->path = D[YM_ASSET_PATH].as<std::string>();
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
		YAML_KEY_VALUE(out, YM_ASSET_PATH, a.second->path.string());
		out << YAML::EndMap;
	}
	out << YAML::EndSeq;

	std::ofstream file{ ADATA_PATH };
	file << out.c_str();
}

void AssetManager::UpdateAData()
{
	// check all assets in resorce folder
	bool isUpdated{false};

	std::vector<std::filesystem::path> files = File::GetFileListsRecv("./Resources");
	for (auto& f:files) {
		if (std::filesystem::is_directory(f) == true) continue;
		auto extension = f.extension();
		AssetType type{ AssetType::None};
		if (extension == PNG) {
			type = AssetType::Texture;
		}
		else if (extension == JPG) {
			type = AssetType::Texture;
		}
		else if (extension == SCN) {
			type = AssetType::Scene;
		}
		else {
			continue;
		}

		bool isNewAsset{true};
		for (auto it = assets.begin(); it != assets.end();++it) {
			if (*(it->second.get()) == f) {
				isNewAsset = false;
				break;
			}
		}
		if (isNewAsset == true) {
			isUpdated = true;
			AssetHandle handle{ UUIDGenerator::Generate(f.string())};// handel generate
			std::shared_ptr<Asset> temp;
			AddAsset(temp, type);
			assets[handle] = temp;
			temp->Handle = handle;
			temp->path = f;
		}
	}



	if (isUpdated == true) {
		SaveAData();
	}
}

AssetType AssetManager::GetAssetType(AssetHandle assetHandle)
{
	if (IsAssetHandleValid(assetHandle) == false) return AssetType::None;
	return latestFoundAsset->second->GetAssetType();
}

std::shared_ptr<Asset> AssetManager::GetAsset(AssetHandle assetHandle)
{
	ENGINE_ASSERT(IsAssetHandleValid(assetHandle) == true, "No asset is there");
	if (latestFoundAsset->second->CheckIsDataLoaded() == false) {
		ENGINE_ASSERT(latestFoundAsset->second->LoadData() == true, "Fail to load asset");
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
	latestFoundAsset = assets.find(assetHandle);
	if (latestFoundAsset == assets.end()) return false;
	return true;
}

bool AssetManager::IsAssetLoaded(AssetHandle handle)
{
	if (IsAssetHandleValid(handle) == false) return false;
	return latestFoundAsset->second->CheckIsDataLoaded();
}

AssetHandle AssetManager::GetHandleFromPath(std::filesystem::path p)
{
	return GetAssetFromPath(p)->Handle;
}

std::shared_ptr<Asset> AssetManager::GetAssetFromPath(std::filesystem::path p)
{
	auto it = assets.begin();
	for (; it != assets.end(); ++it) {
		if (*(it->second.get()) == p) {
			break;
		}
	}
	ENGINE_ASSERT(it != assets.end(),"Path is not an asset");
	return it->second;
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
