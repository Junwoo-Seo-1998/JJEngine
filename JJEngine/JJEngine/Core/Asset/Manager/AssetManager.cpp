#include "AssetManager.h"
#include "Core/Utils/YAML_IMPL.hpp"
#include "Core/Utils/File.h"
#include "Core/Utils/UUIDGenerator.h"
#include "Core/Asset/Metadata.h"

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

void AssetManager::GenAsset(std::shared_ptr<Asset>& empty_asset, AssetType type)
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
		GenAsset(temp,type);
		assets[handle] = temp;
		temp->Handle = handle;

		std::shared_ptr<Metadata> Meta_temp = std::make_shared<Metadata>();
		assetMetadatas[handle] = Meta_temp;
		Meta_temp->Handle = handle;
		Meta_temp->path = D[YM_ASSET_PATH].as<std::string>();
		Meta_temp->type = type;
		Meta_temp->isMemoryOnlyAsset = false;
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
	for (auto& a : assetMetadatas) {
		if (a.second->isMemoryOnlyAsset == true) continue;
		out << YAML::BeginMap;
		YAML_KEY_VALUE(out, YM_ASSET_HANDLE, a.second->Handle);
		YAML_KEY_VALUE(out, YM_ASSET_TYPE, static_cast<int>(a.second->type));
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
		for (auto it = assetMetadatas.begin(); it != assetMetadatas.end();++it) {
			if (it->second->path == f) {
				isNewAsset = false;
				break;
			}
		}
		if (isNewAsset == true) {
			isUpdated = true;
			AssetHandle handle{ UUIDGenerator::Generate(f.string())};// handel generate
			std::shared_ptr<Asset> temp;
			GenAsset(temp, type);
			assets[handle] = temp;
			temp->Handle = handle;

			std::shared_ptr<Metadata> Meta_temp = std::make_shared<Metadata>();
			assetMetadatas[handle] = Meta_temp;
			Meta_temp->Handle = handle;
			Meta_temp->path = f;
			Meta_temp->type = type;
			Meta_temp->isMemoryOnlyAsset = false;
		}
	}

	std::vector<AssetHandle> deletedAssets{};
	for (auto it = assetMetadatas.begin(); it != assetMetadatas.end(); ++it) {
		if (it->second->isMemoryOnlyAsset == true) continue;
		if (File::CheckExists(it->second->path)== false) {
			deletedAssets.push_back(it->second->Handle);
		}
	}
	for (auto & d:deletedAssets) {
		assetMetadatas.erase(d);
		assets.erase(d);
	}

	if (isUpdated == true) {
		SaveAData();
	}
}

AssetHandle AssetManager::AddMemoryOnlyAsset(AssetType type)
{
	static int dynamicID{0};
	std::string path{"Resources/MemoryOnlyAsset(" + std::to_string(dynamicID) + ")"};
	AssetHandle handle{ UUIDGenerator::Generate(path)};
	std::shared_ptr<Asset> temp;
	GenAsset(temp, type);
	assets[handle] = temp;
	temp->Handle = handle;

	std::shared_ptr<Metadata> Meta_temp = std::make_shared<Metadata>();
	assetMetadatas[handle] = Meta_temp;
	Meta_temp->Handle = handle;
	Meta_temp->path = path;
	Meta_temp->type = type;
	Meta_temp->isMemoryOnlyAsset = true;

	return handle;
}

bool AssetManager::IsMemoryOnlyAsset(AssetHandle handle)
{
	ENGINE_ASSERT(IsAssetHandleValid(handle) == true, "Asset handle is not valid");
	return assetMetadatas[handle]->isMemoryOnlyAsset;
}

std::shared_ptr<Metadata> AssetManager::GetMetadata(AssetHandle assetHandle)
{
	ENGINE_ASSERT(IsAssetHandleValid(assetHandle) == true, "Asset handle is not valid");
	return assetMetadatas[assetHandle];
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
		ENGINE_ASSERT(latestFoundAsset->second->LoadData(assetMetadatas[assetHandle]->path) == true, "Fail to load asset");
	}
	return latestFoundAsset->second;
}

bool AssetManager::LoadData(AssetHandle assetHandle)
{
	if (IsAssetHandleValid(assetHandle) == false) return false;
	if (latestFoundAsset->second->LoadData(assetMetadatas[assetHandle]->path) == false) return false;
	return true;
}

bool AssetManager::ReloadData(AssetHandle assetHandle)
{
	if (IsAssetHandleValid(assetHandle) == false) return false;
	latestFoundAsset->second->UnloadData();
	if (latestFoundAsset->second->LoadData(assetMetadatas[assetHandle]->path) == false) return false;
	return true;
}

bool AssetManager::UnloadData(AssetHandle assetHandle)
{
	if (IsAssetHandleValid(assetHandle) == false) return false;
	latestFoundAsset->second->UnloadData();
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
	auto it = assetMetadatas.begin();
	for (; it != assetMetadatas.end(); ++it) {
		if (it->second->path == p) {
			break;
		}
	}
	if (it == assetMetadatas.end())return AssetHandle{};
	return it->second->Handle;
}

std::shared_ptr<Asset> AssetManager::GetAssetFromPath(std::filesystem::path p)
{
	AssetHandle temp = GetHandleFromPath(p);
	ENGINE_ASSERT(temp != AssetHandle{}, "Path is not an asset");
	return GetAsset(temp);
}

std::unordered_set<AssetHandle> AssetManager::GetAllMemoryOnlyAssetHandles()
{
	std::unordered_set<AssetHandle> handles{};
	for (auto& a : assetMetadatas) {
		if (a.second->isMemoryOnlyAsset == true) {
			handles.insert(a.second->Handle);
		}
	}
	return handles;
}

std::unordered_set<AssetHandle> AssetManager::GetAllFileAssetHandles()
{
	std::unordered_set<AssetHandle> handles{};
	for (auto& a : assetMetadatas) {
		if (a.second->isMemoryOnlyAsset == false) {
			handles.insert(a.second->Handle);
		}
	}
	return handles;
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

const std::unordered_map<AssetHandle, std::shared_ptr<Metadata>>& AssetManager::GetAllMetadatas()
{
	return assetMetadatas;
}
