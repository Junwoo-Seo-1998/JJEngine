#pragma once
#include "AssetType.h"
#include "uuid.h"
#include <filesystem>

using AssetHandle = uuids::uuid;

class Asset
{
protected:
	friend class AssetManager;

	virtual bool CheckIsDataLoaded() const = 0;
	virtual bool LoadData() = 0;
	virtual void UnloadData() = 0;

	AssetHandle Handle;
	std::filesystem::path path;
	//void* dataPtr{};
	// loaded time?
public:


	virtual ~Asset() = default;
	static AssetType GetStaticAssetType() { return AssetType::None; }
	virtual AssetType GetAssetType() const { return AssetType::None; }
	const AssetHandle GetHandle() const { return Handle; }

	virtual bool operator==(const Asset& other) const
	{
		return Handle == other.Handle;
	}

	virtual bool operator!=(const Asset& other) const
	{
		return !(*this == other);
	}

	virtual bool operator==(const AssetHandle& other) const
	{
		return Handle == other;
	}

	virtual bool operator!=(const AssetHandle& other) const
	{
		return !(*this == other);
	}

	virtual bool operator==(const std::filesystem::path& other) const
	{
		return path == other;
	}

	virtual bool operator!=(const std::filesystem::path& other) const
	{
		return !(*this == other);
	}

};

//bool operator==(const std::shared_ptr<Asset>& ptr, const AssetHandle& other)
//{
//	return *ptr.get() == other;
//}
//bool operator!=(const std::shared_ptr<Asset>& ptr, const AssetHandle& other)
//{
//	return !(*ptr.get() == other);
//}

//bool operator==(const Asset& self, const Asset& other) 
//{
//	return self.GetHandle() == other.GetHandle();
//}
//
// bool operator!=(const Asset& self, const Asset& other) 
//{
//	return !(self == other);
//}
//
// bool operator==(const Asset& self, const AssetHandle& other) 
//{
//	return Handle == other;
//}
//
// bool operator!=(const Asset& self, const AssetHandle& other) 
//{
//	return !(*this == other);
//}
//
// bool operator==(const Asset& self, const std::filesystem::path& other) 
//{
//	return path == other;
//}
//
// bool operator!=(const Asset& self, const std::filesystem::path& other) 
//{
//	return !(*this == other);
//}