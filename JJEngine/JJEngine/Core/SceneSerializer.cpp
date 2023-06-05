#include "SceneSerializer.h"
#include "Scene.h"
#include "Entity/Entity.hpp"
#include <fstream>
#include <yaml-cpp/yaml.h>
#include "Core/Component/TransformComponent.h"
#include "Core/Entity/RelationshipComponent.h"
#include "Core/Utils/Log.h"

// Key values
#define YM_SCENE "Scene"
#define YM_ENTITY "Entities"
#define YM_NAME "Name"
#define YM_UUID "UUID"
#define YM_COMPONENT "Components"
#define YM_TRANSFORM "Transform"
#define YM_POSITION "Position"
#define YM_ROTATION "Rotation"
#define YM_SCALE "Scale"
#define YM_RELATIONSHIP "Relationship"
#define YM_PARENT "Parent"
#define YM_CHILDREN "Children"
#define YM_SPRITE "Sprite"

// Emitter macro
#define YAML_KEY_VALUE(emitter, key, value) emitter<<YAML::Key<<key<<YAML::Value<<value;

// YAML Implement
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

	Emitter& operator<<(Emitter& emitter, UUIDType v) {
		return emitter << to_string(v);
	}

	template<>
	struct convert<UUIDType> {
		static Node encode(const UUIDType& rhs) {
			Node node;
			node.push_back(to_string(rhs));
			return node;
		}
		static bool decode(const Node& node, UUIDType& rhs) {
			if (node.IsScalar() == false) return false;
			rhs = UUIDType::from_string(node.as<std::string>()).value();
			return true;
		}
	};
}

// Helper function
void SerializeEntity(YAML::Emitter& out, entt::entity ID, std::shared_ptr<Scene> scene);
void DeserializeRelationship(YAML::iterator::value_type& component, std::shared_ptr<Scene> scene);
void DeserializeTransform(YAML::iterator::value_type& transform_components, std::shared_ptr<Scene> scene);
void DeserializeEntity(YAML::detail::iterator_value entity, std::shared_ptr<Scene> scene);

SceneSerializer::SceneSerializer(std::shared_ptr<Scene> sc): scene(sc)
{
}

void SceneSerializer::Serialize(const std::string filePath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << YM_SCENE;
	out << YAML::Value << scene->m_scene_name;
	out << YAML::Key << YM_ENTITY;
	out << YAML::Value << YAML::BeginSeq;
	scene->m_Registry.each([&](auto entityID)
	{
		SerializeEntity(out,entityID,scene);
	});
	out << YAML::EndSeq;

	out << YAML::Key << YM_COMPONENT;
	out << YAML::Value << YAML::BeginMap;
	{
		{
			out << YAML::Key << YM_RELATIONSHIP;
			out << YAML::Value << YAML::BeginMap;
			auto components = scene->m_Registry.view<RelationshipComponent>();
			for (auto c : components)
			{
				Entity entity{ c,scene.get() };
				YAML_KEY_VALUE(out, entity.GetUUID(), YAML::BeginMap)
					YAML_KEY_VALUE(out, YM_PARENT, entity.GetParentUUID())
					YAML_KEY_VALUE(out, YM_CHILDREN, YAML::BeginSeq)
					auto& children = entity.GetChildrenUUID();
				for (auto& id : children) {
					out << id;
				}
				out << YAML::EndSeq;
				out << YAML::EndMap;
			}
			out << YAML::EndMap;
		}
		{
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
		}
		{
			out << YAML::Key << YM_SPRITE;
			out << YAML::Value << YAML::BeginMap;
			out << YAML::EndMap;
		}
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

	scene->m_scene_name = data[YM_SCENE].as<std::string>();

	auto entities = data[YM_ENTITY];
	for (auto e: entities) {
		DeserializeEntity(e, scene);
	}
	auto components = data[YM_COMPONENT];
	{
		auto compos = components[YM_RELATIONSHIP];
		for (auto c : compos) {
			DeserializeRelationship(c, scene);
		}
	}
	{
		auto compos = components[YM_TRANSFORM];
		for (auto c : compos) {
			DeserializeTransform(c, scene);
		}
	}

	return true;
}












void SerializeEntity(YAML::Emitter& out, entt::entity ID, std::shared_ptr<Scene> scene) {
	Entity entity{ ID, scene.get() };
	out << YAML::BeginMap;
	YAML_KEY_VALUE(out,YM_NAME, entity.Name())
	YAML_KEY_VALUE(out,YM_UUID, entity.GetUUID())
	out << YAML::EndMap;
}
void DeserializeRelationship(YAML::iterator::value_type& component, std::shared_ptr<Scene> scene) {
	Entity entity = scene->GetEntity(component.first.as<UUIDType>());
	UUIDType parent{ component.second[YM_PARENT].as<UUIDType>() };
	entity.SetParentUUID(parent);
	std::vector<UUIDType>& children = entity.GetComponent<RelationshipComponent>().Children;
	for (auto c: component.second[YM_CHILDREN]) {
		children.emplace_back(c.as<UUIDType>());
	}
}
void DeserializeTransform(YAML::iterator::value_type& component, std::shared_ptr<Scene> scene) {
	Entity entity = scene->GetEntity(component.first.as<UUIDType>());
	glm::vec3 pos{ component.second[YM_POSITION].as<glm::vec3>()};
	glm::vec3 rotation{ component.second[YM_ROTATION].as<glm::vec3>()};
	glm::vec3 scale{ component.second[YM_SCALE].as<glm::vec3>() };
	entity.Transform().Position = pos;
	entity.Transform().Rotation = rotation;
	entity.Transform().Scale = scale;
}
void DeserializeEntity(YAML::detail::iterator_value entity, std::shared_ptr<Scene> scene) {
	std::string name{ entity[YM_NAME].as<std::string>() };
	UUIDType id = entity[YM_UUID].as<UUIDType>();
	Entity p_e = scene->CreateEntityWithUUID(id, name);
}