#include "Asset_Mesh.h"
#include "Core/Graphics/MeshFactory.h"
#include <fstream>

#define MFDATA ".MFData"

enum class MeshFactoryType
{
	None = 0,
	Box,
	SkyBox,
	Count
};

bool Asset_Mesh::CheckIsDataLoaded() const
{
	return data != nullptr;
}

bool Asset_Mesh::LoadData(std::filesystem::path p)
{
	if (p.extension() == MFDATA) {
		std::ifstream file{p};
		int type{ file.get()};
		MeshFactoryDataLoad(type);
		file.close();
		return true;
	}
	return false;
}

void Asset_Mesh::UnloadData()
{
	data = nullptr;
}


void Asset_Mesh::MeshFactoryDataLoad(int type) {
	switch (static_cast<MeshFactoryType>(type))
	{
	case MeshFactoryType::Box:
		data = MeshFactory::CreateBox({ 1.f,1.f,1.f });
		break;
	case MeshFactoryType::SkyBox:
		data = MeshFactory::CreateSkyCube({ 1.f,1.f,1.f });
		break;
	default:
		break;
	}
}