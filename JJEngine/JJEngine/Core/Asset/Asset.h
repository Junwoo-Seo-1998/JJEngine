#pragma once
#include "AssetType.h"
#include "uuid.h"
using AssetHandle = uuids::uuid;

class Asset
{
public:
	AssetHandle Handle;

	virtual ~Asset() = default;

	static AssetType GetStaticAssetType() { return AssetType::None; }
	virtual AssetType GetAssetType() const { return AssetType::None; }

	virtual bool operator==(const Asset& other) const
	{
		return Handle == other.Handle;
	}

	virtual bool operator!=(const Asset& other) const
	{
		return !(*this == other);
	}
};
