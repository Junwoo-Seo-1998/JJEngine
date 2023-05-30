#include "SceneSerializer.h"
#include "Scene.h"
#include "Entity/Entity.hpp"
#include <fstream>
#include <yaml-cpp/yaml.h>

SceneSerializer::SceneSerializer(std::shared_ptr<Scene> sc): scene(sc)
{
}

void SerializeEntityTree(YAML::Emitter& out, entt::entity ID, std::shared_ptr<Scene> scene);

void SceneSerializer::Serialize(const std::string filePath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene";
	out << YAML::Value << "Name";
	out << YAML::Key << "Entities";
	out << YAML::Value << YAML::BeginSeq;
	std::vector < entt::entity > rootEntity{};
	scene->m_Registry.each([&](auto entityID)
	{
		Entity entity{ entityID, scene.get() };
		if (entity.GetParentUUID().is_nil() == true) {
			rootEntity.push_back(entityID);
		}
	});

	for (auto& e : rootEntity) {
		SerializeEntityTree(out, e, scene);
	}
	out << YAML::EndSeq;

	std::ofstream file{ filePath };
	file << out.c_str();
}

void DeserializeEntity(YAML::detail::iterator_value entity, std::shared_ptr<Scene> scene);

bool SceneSerializer::Deserialize(const std::string filePath)
{
	std::ifstream file{filePath};
	std::stringstream strStream{};
	strStream << file.rdbuf();

	YAML::Node data = YAML::Load(strStream.str());
	if (!data["Scene"]) return false;

	//std::string sceneName{data["Scene"].as<std::string>()};

	auto entities = data["Entities"];
	for (auto e: entities) {
		DeserializeEntity(e,scene);
	}


	return true;
}



void SerializeEntityTree(YAML::Emitter& out, entt::entity ID, std::shared_ptr<Scene> scene) {
	Entity entity{ ID, scene.get() };
	out<< YAML::BeginMap;
	out << YAML::Key << "Name";
	out << YAML::Value << entity.Name();

	//components
	//

	{//Hierarchy
		std::vector<UUIDType>& children = entity.GetChildrenUUID();
		out << YAML::Key << "Children";
		out << YAML::Value << YAML::BeginSeq;
		for (auto& c : children) {
			SerializeEntityTree(out, scene->GetEntity(c).GetEntityHandle(),scene);
		}
		out << YAML::EndSeq;
	}


	out<< YAML::EndMap;
}

void DeserializeEntityTree(Entity &parent, YAML::detail::iterator_value entity, std::shared_ptr<Scene> scene);

void DeserializeEntity(YAML::detail::iterator_value entity, std::shared_ptr<Scene> scene) {
	std::string Name{ entity["Name"].as<std::string>() };
	Entity p_e = scene->CreateEntity(Name);

	auto children = entity["Children"];
	for (auto c : children) {
		DeserializeEntityTree(p_e, c, scene);
	}
}

void DeserializeEntityTree(Entity &parent, YAML::detail::iterator_value entity, std::shared_ptr<Scene> scene) {
	std::string Name{entity["Name"].as<std::string>()};
	Entity new_p_e = scene->CreateEntity(Name);
	new_p_e.SetParent(parent);

	auto children = entity["Children"];
	for (auto c : children) {
		DeserializeEntityTree(new_p_e, c, scene);
	}
}