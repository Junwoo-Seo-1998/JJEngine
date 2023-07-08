#include "Asset_Mesh.h"
#include "Core/Graphics/MeshFactory.h"
#include <fstream>
#include <yaml-cpp/yaml.h>
#include "Core/Utils/Assert.h"

#define MFDATA ".MFData"
#define TYPE 

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
		std::ifstream file{ p };
		std::stringstream strStream{};
		strStream << file.rdbuf();
		YAML::Node data = YAML::Load(strStream.str());
		int type{ data["Type"].as<int>() };
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
		ENGINE_ASSERT(false, "Not expected Mesh Factory type");
		break;
	}
}