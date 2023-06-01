#include "SceneSerializer.h"
#include "Scene.h"
#include "Entity/Entity.hpp"
#include <fstream>
#include <yaml-cpp/yaml.h>
#include "Core/Component/TransformComponent.h"
#include "Core/Entity/RelationshipComponent.h"
#include "Core/Utils/Log.h"

#define YM_SCENE "Scene"
#define YM_NAME "Name"
#define YM_ENTITY "Entities"
#define YM_COMPONENT "Components"
#define YM_TRANSFORM "Transform"
#define YM_POSITION "Position"
#define YM_ROTATION "Rotation"
#define YM_SCALE "Scale"
#define YM_SPRITE "Sprite"
#define YM_UUID "UUID"
#define YM_CHILDREN "Children"
#define YAML_KEY_VALUE(emitter, key, value) emitter<<YAML::Key<<key<<YAML::Value<<value;

namespace YAML {
	Emitter& operator<<(Emitter& emitter, glm::vec3 v) {
		return emitter << BeginSeq << v.x << v.y << v.z << EndSeq;
	}

	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}
		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
}
void DeserializeTransform(Entity& entity, std::string id, YAML::Node& transform_components);
void DeserializeEntity(YAML::detail::iterator_value entity, YAML::Node& components, std::shared_ptr<Scene> scene);
void DeserializeEntityTree(Entity &parent, YAML::detail::iterator_value entity, YAML::Node& components, std::shared_ptr<Scene> scene);
void SerializeEntityTree(YAML::Emitter& out, entt::entity ID, std::shared_ptr<Scene> scene);

SceneSerializer::SceneSerializer(std::shared_ptr<Scene> sc): scene(sc)
{
}

void SceneSerializer::Serialize(const std::string filePath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << YM_SCENE;
	out << YAML::Value << YM_NAME;
	out << YAML::Key << YM_ENTITY;
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

	out << YAML::Key << YM_COMPONENT;
	out << YAML::Value << YAML::BeginMap;
	{	
		out << YAML::Key << YM_NAME;
		out << YAML::Value << YAML::BeginMap;
		out << YAML::EndMap;

		out << YAML::Key << YM_TRANSFORM;
		out << YAML::Value << YAML::BeginMap;
		auto components = scene->m_Registry.view<TransformComponent>();
		for (auto c : components) 
		{
			Entity entity{ c,scene.get() };
			YAML_KEY_VALUE(out, to_string(entity.GetUUID()), YAML::BeginMap)
				YAML_KEY_VALUE(out, YM_POSITION, entity.Transform().Position)
				YAML_KEY_VALUE(out, YM_ROTATION, entity.Transform().Rotation)
				YAML_KEY_VALUE(out, YM_SCALE, entity.Transform().Scale)
			out << YAML::EndMap;
		}
		out << YAML::EndMap;

		//out << YAML::BeginMap;
		//auto components = scene->m_Registry.view<TransformComponent>();
		//for (auto c : components) {
		//	Entity entity{ c, scene.get() };
		//	out << YAML::Key << to_string(entity.GetUUID());
		//	out << YAML::Value << YAML::BeginMap; 
		//	out << YAML::Key << "Position";
		//	out << YAML::Value << entity.Transform().Position;
		//	out << YAML::Key << "Rotation";
		//	out << YAML::Value << entity.Transform().Rotation;
		//	out << YAML::Key << "Scale";
		//	out << YAML::Value << entity.Transform().Scale;
		//	out << YAML::EndMap;
		//}
		//out << YAML::EndMap;

		out << YAML::Key << YM_SPRITE;
		out << YAML::Value << YAML::BeginMap;
		out << YAML::EndMap;
	}
	out << YAML::EndMap;

	std::ofstream file{ filePath };
	file << out.c_str();
}


bool SceneSerializer::Deserialize(const std::string filePath)
{
	std::ifstream file{filePath};
	std::stringstream strStream{};
	strStream << file.rdbuf();

	YAML::Node data = YAML::Load(strStream.str());
	if (!data[YM_SCENE]) return false;

	//std::string sceneName{data["Scene"].as<std::string>()};

	auto entities = data[YM_ENTITY];
	auto components = data[YM_COMPONENT];
	for (auto e: entities) {
		DeserializeEntity(e, components ,scene);
	}


	return true;
}



void SerializeEntityTree(YAML::Emitter& out, entt::entity ID, std::shared_ptr<Scene> scene) {
	Entity entity{ ID, scene.get() };
	out << YAML::BeginMap;
	out << YAML::Key << YM_NAME;
	out << YAML::Value << entity.Name();

	{//UUID
		out << YAML::Key << YM_UUID;
		out << YAML::Value << to_string(entity.GetUUID());
	}

	{//Hierarchy
		std::vector<UUIDType>& children = entity.GetChildrenUUID();
		out << YAML::Key << YM_CHILDREN;
		out << YAML::Value << YAML::BeginSeq;
		for (auto& c : children) {
			SerializeEntityTree(out, scene->GetEntity(c).GetEntityHandle(),scene);
		}
		out << YAML::EndSeq;
	}

	out<< YAML::EndMap;
}

void DeserializeTransform(Entity& entity,std::string id, YAML::Node& transform_components) {

	glm::vec3 pos{ transform_components[id][YM_POSITION].as<glm::vec3>()};
	glm::vec3 rotation{ transform_components[id][YM_ROTATION].as<glm::vec3>()};
	glm::vec3 scale{ transform_components[id][YM_SCALE].as<glm::vec3>() };
	entity.Transform().Position = pos;
	entity.Transform().Rotation = rotation;
	entity.Transform().Scale = scale;

}
void DeserializeEntity(YAML::detail::iterator_value entity, YAML::Node& components, std::shared_ptr<Scene> scene) {
	std::string Name{ entity[YM_NAME].as<std::string>() };
	Entity p_e = scene->CreateEntity(Name);

	auto transformCompos{ components[YM_TRANSFORM] };
	DeserializeTransform(p_e, entity[YM_UUID].as<std::string>(), transformCompos);

	auto children = entity[YM_CHILDREN];
	for (auto c : children) {
		DeserializeEntityTree(p_e, c, components, scene);
	}
}

void DeserializeEntityTree(Entity &parent, YAML::detail::iterator_value entity, YAML::Node& components, std::shared_ptr<Scene> scene) {
	std::string Name{entity[YM_NAME].as<std::string>()};
	Entity new_p_e = scene->CreateEntity(Name);
	new_p_e.SetParent(parent);

	auto transformCompos{ components[YM_TRANSFORM] };
	DeserializeTransform(new_p_e, entity[YM_UUID].as<std::string>(), transformCompos);

	auto children = entity[YM_CHILDREN];
	for (auto c : children) {
		DeserializeEntityTree(new_p_e, c, components, scene);
	}
}