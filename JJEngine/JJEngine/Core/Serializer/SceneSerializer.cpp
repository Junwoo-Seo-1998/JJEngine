#include "SceneSerializer.h"
#include "Core/Scene.h"
#include "Core/Entity/Entity.hpp"
#include <fstream>
#include "Core/Component/TransformComponent.h"
#include "Core/Entity/RelationshipComponent.h"
#include "Core/Utils/Log.h"
#include "Core/Component/SpriteRendererComponent.h"
#include "Core/Component/CameraComponent.h"
#include "Core/Component/RigidBody2DComponent.h"
#include "Core/Component/BoxCollider2DComponent.h"
#include "Core/Component/ScriptComponent.h"
#include "Core/Component/LightComponent.h"
#include "Core/Component/MeshComponent.h"

#include "Core/Utils/YAML_IMPL.hpp"
#include "Core/Asset/Asset_Texture.h"
#include "Core/Application.h"
#include "Core/Asset/Manager/AssetManager.h"

// Key values
#define YM_SCENE "Scene"
#define YM_ENTITY "Entities"
#define YM_NAME "Name"
#define YM_UUID "UUID"
#define YM_COMPONENT "Components"
#define YM_TRANSFORM "Transform"
#define YM_SCRIPT "Script"
#define YM_SCRIPT_NAME "ScriptName"
#define YM_POSITION "Position"
#define YM_ROTATION "Rotation"
#define YM_SCALE "Scale"
#define YM_RELATIONSHIP "Relationship"
#define YM_PARENT "Parent"
#define YM_CHILDREN "Children"
#define YM_SPRITE "Sprite"
#define YM_SPRITE_HANDLE "SpriteHandle"
#define YM_SPRITE_COLOR "SpriteColor"
#define YM_CAMERA "Camera"
#define YM_CAMERAVALUES "CameraValues"
#define YM_CAMERABOOLS "CameraBools"
#define YM_RIGIDBODY_2D "RigidBody2D"
#define YM_RIGIDBODY_2D_TYPE "RigidBody2DType"
#define YM_RIGIDBODY_2D_BOOL "RigidBody2DBool"
#define YM_BOXCOLLIDER_2D "BoxCollider2D"
#define YM_BOXCOLLIDER_2D_2DVALUES "BoxCollider2D_2DValues"
#define YM_BOXCOLLIDER_2D_1DVALUES "BoxCollider2D_1DValues"
#define YM_LIGHT "Light"
#define YM_LIGHT_TYPE "LightType"
#define YM_LIGHT_ANGLEINNER "LightAngleInner"
#define YM_LIGHT_ANGLEOUTER "LightAngleOuter"
#define YM_LIGHT_AMBIENT "LightAmbient"
#define YM_LIGHT_DIFFUSE "LightDiffuse"
#define YM_LIGHT_SPECULAR "LightSpecular"
#define YM_LIGHT_FALLOFF "LightFalloff"
#define YM_MESH "Mesh"
#define YM_MESH_HANDLE "MeshHandle"

// Helper function
void SerializeEntity(YAML::Emitter& out, entt::entity ID, std::shared_ptr<Scene> scene);
void DeserializeRelationship(YAML::iterator::value_type& component, std::shared_ptr<Scene> scene);
void DeserializeTransform(YAML::iterator::value_type& transform_components, std::shared_ptr<Scene> scene);
void DeserializeScript(YAML::iterator::value_type& component, std::shared_ptr<Scene> scene);
void DeserializeSprite(YAML::iterator::value_type& component, std::shared_ptr<Scene> scene);
void DeserializeCamera(YAML::iterator::value_type& component, std::shared_ptr<Scene> scene);
void DeserializeRidgidBody2D(YAML::iterator::value_type& component, std::shared_ptr<Scene> scene);
void DeserializeBox2D(YAML::iterator::value_type& component, std::shared_ptr<Scene> scene);
void DeserializeLight(YAML::iterator::value_type& component, std::shared_ptr<Scene> scene);
void DeserializeMesh(YAML::iterator::value_type& component, std::shared_ptr<Scene> scene);
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
				YAML_KEY_VALUE(out, entity.GetUUID(), YAML::BeginMap);
				YAML_KEY_VALUE(out, YM_PARENT, entity.GetParentUUID());
				YAML_KEY_VALUE(out, YM_CHILDREN, YAML::BeginSeq);
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
				YAML_KEY_VALUE(out, to_string(entity.GetUUID()), YAML::BeginMap);
				YAML_KEY_VALUE(out, YM_POSITION, entity.Transform().Position);
				YAML_KEY_VALUE(out, YM_ROTATION, entity.Transform().Rotation);
				YAML_KEY_VALUE(out, YM_SCALE, entity.Transform().Scale);
					out << YAML::EndMap;
			}
			out << YAML::EndMap;
		}

		{
			out << YAML::Key << YM_SCRIPT;
			out << YAML::Value << YAML::BeginMap;
			auto components = scene->m_Registry.view<ScriptComponent>();
			for (auto c : components)
			{
				Entity entity{ c,scene.get() };
				YAML_KEY_VALUE(out, to_string(entity.GetUUID()), YAML::BeginMap);
				YAML_KEY_VALUE(out, YM_SCRIPT_NAME, components.get<ScriptComponent>(c).Name);
				out << YAML::EndMap;
			}
			out << YAML::EndMap;
		}

		{
			out << YAML::Key << YM_SPRITE;
			out << YAML::Value << YAML::BeginMap;
			auto components = scene->m_Registry.view<SpriteRendererComponent>();
			for (auto c : components)
			{
				Entity entity{ c,scene.get() };
				YAML_KEY_VALUE(out, to_string(entity.GetUUID()), YAML::BeginMap);
				YAML_KEY_VALUE(out, YM_SPRITE_HANDLE, (components.get<SpriteRendererComponent>(c).asset ? components.get<SpriteRendererComponent>(c).asset->GetHandle() : AssetHandle{}));
				YAML_KEY_VALUE(out, YM_SPRITE_COLOR, components.get<SpriteRendererComponent>(c).color);
				out << YAML::EndMap;
			}
			out << YAML::EndMap;
		}
		{
			out << YAML::Key << YM_CAMERA;
			out << YAML::Value << YAML::BeginMap;
			auto components = scene->m_Registry.view<CameraComponent>();
			for (auto c : components)
			{
				Entity entity{ c,scene.get() };
				YAML_KEY_VALUE(out, to_string(entity.GetUUID()), YAML::BeginMap);
				CameraComponent& cacom = components.get<CameraComponent>(c);
				YAML_KEY_VALUE(out, YM_CAMERAVALUES, YAML::BeginSeq);
				out << cacom.cam_speed << cacom.Fov_y << cacom.Aspect_ratio << cacom.Near << cacom.Far << YAML::EndSeq;
				YAML_KEY_VALUE(out, YM_CAMERABOOLS, YAML::BeginSeq);
				out << cacom.IsMainCamera << YAML::EndSeq;
				out << YAML::EndMap;
			}
			out << YAML::EndMap;
		}
		{
			out << YAML::Key << YM_RIGIDBODY_2D;
			out << YAML::Value << YAML::BeginMap;
			auto components = scene->m_Registry.view<RigidBody2DComponent>();
			for (auto c : components)
			{
				Entity entity{ c,scene.get() };
				YAML_KEY_VALUE(out, to_string(entity.GetUUID()), YAML::BeginMap);
				YAML_KEY_VALUE(out, YM_RIGIDBODY_2D_TYPE, static_cast<int>(components.get<RigidBody2DComponent>(c).Type));
				YAML_KEY_VALUE(out, YM_RIGIDBODY_2D_BOOL, components.get<RigidBody2DComponent>(c).FixedRotation);
				out << YAML::EndMap;
			}
			out << YAML::EndMap;
		}
		{
			out << YAML::Key << YM_BOXCOLLIDER_2D;
			out << YAML::Value << YAML::BeginMap;
			auto components = scene->m_Registry.view<BoxCollider2DComponent>();
			for (auto c : components)
			{
				Entity entity{ c,scene.get() };
				YAML_KEY_VALUE(out, to_string(entity.GetUUID()), YAML::BeginMap);
				BoxCollider2DComponent& bocom = components.get<BoxCollider2DComponent>(c);
				YAML_KEY_VALUE(out, YM_BOXCOLLIDER_2D_2DVALUES, YAML::BeginSeq);
				out << bocom.Offset << bocom.Size << YAML::EndSeq;
				YAML_KEY_VALUE(out, YM_BOXCOLLIDER_2D_1DVALUES, YAML::BeginSeq);
				out << bocom.Density << bocom.Friction << bocom.Restitution << bocom.RestitutionThreshold;
				out << YAML::EndSeq;
				out << YAML::EndMap;
			}
			out << YAML::EndMap;
		}
		{
			out << YAML::Key << YM_LIGHT;
			out << YAML::Value << YAML::BeginMap;
			auto components = scene->m_Registry.view<LightComponent>();
			for (auto c : components)
			{
				Entity entity{ c,scene.get() };
				YAML_KEY_VALUE(out, to_string(entity.GetUUID()), YAML::BeginMap);
				LightComponent& light = components.get<LightComponent>(c);
				YAML_KEY_VALUE(out, YM_LIGHT_TYPE, static_cast<int>(light.light.m_LightType));
				YAML_KEY_VALUE(out, YM_LIGHT_ANGLEINNER, light.light.m_Angle.inner);
				YAML_KEY_VALUE(out, YM_LIGHT_ANGLEOUTER, light.light.m_Angle.outer);
				YAML_KEY_VALUE(out, YM_LIGHT_AMBIENT, light.light.Ambient);
				YAML_KEY_VALUE(out, YM_LIGHT_DIFFUSE, light.light.Diffuse);
				YAML_KEY_VALUE(out, YM_LIGHT_SPECULAR, light.light.Specular);
				YAML_KEY_VALUE(out, YM_LIGHT_FALLOFF, light.light.falloff);
				out << YAML::EndMap;
			}
			out << YAML::EndMap;
		}
		{
			out << YAML::Key << YM_MESH;
			out << YAML::Value << YAML::BeginMap;
			auto components = scene->m_Registry.view<MeshComponent>();
			for (auto c : components)
			{
				Entity entity{ c,scene.get() };
				YAML_KEY_VALUE(out, to_string(entity.GetUUID()), YAML::BeginMap);
				MeshComponent& comp = components.get<MeshComponent>(c);
				YAML_KEY_VALUE(out, YM_MESH_HANDLE, comp.handle);
				out << YAML::EndMap;
			}
			out << YAML::EndMap;
		}
	}
	out << YAML::EndMap;

	std::ofstream file{ filePath };
	file << out.c_str();
	file.close();
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
	{
		auto compos = components[YM_SCRIPT];
		for (auto c : compos) {
			DeserializeScript(c, scene);
		}
	}
	{
		auto compos = components[YM_SPRITE];
		for (auto c : compos) {
			DeserializeSprite(c, scene);
		}
	}
	{
		auto compos = components[YM_CAMERA];
		for (auto c : compos) {
			DeserializeCamera(c, scene);
		}
	}
	{
		auto compos = components[YM_RIGIDBODY_2D];
		for (auto c : compos) {
			DeserializeRidgidBody2D(c, scene);
		}
	}
	{
		auto compos = components[YM_BOXCOLLIDER_2D];
		for (auto c : compos) {
			DeserializeBox2D(c, scene);
		}
	}
	{
		auto compos = components[YM_LIGHT];
		for (auto c : compos) {
			DeserializeLight(c, scene);
		}
	}
	{
		auto compos = components[YM_MESH];
		for (auto c : compos) {
			DeserializeMesh(c, scene);
		}
	}
	file.close();
	return true;
}












void SerializeEntity(YAML::Emitter& out, entt::entity ID, std::shared_ptr<Scene> scene) {
	Entity entity{ ID, scene.get() };
	out << YAML::BeginMap;
	YAML_KEY_VALUE(out, YM_NAME, entity.Name());
	YAML_KEY_VALUE(out, YM_UUID, entity.GetUUID());
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

void DeserializeScript(YAML::iterator::value_type& component, std::shared_ptr<Scene> scene) {
	Entity entity = scene->GetEntity(component.first.as<UUIDType>());
	std::string name = component.second[YM_SCRIPT_NAME].as<std::string>();
	ScriptComponent& temp = entity.AddComponent<ScriptComponent>();
	temp.Name = name;
}

void DeserializeSprite(YAML::iterator::value_type& component, std::shared_ptr<Scene> scene) {
	Entity entity = scene->GetEntity(component.first.as<UUIDType>());
	glm::vec4 col{ component.second[YM_SPRITE_COLOR].as<glm::vec4>() };
	SpriteRendererComponent& temp = entity.AddComponent<SpriteRendererComponent>(col);
	if (component.second[YM_SPRITE_HANDLE].as<AssetHandle>().is_nil() == false)
		temp.asset = Application::Instance().GetAssetManager()->GetCastedAsset<Asset_Texture>(component.second[YM_SPRITE_HANDLE].as<AssetHandle>());
}
void DeserializeCamera(YAML::iterator::value_type& component, std::shared_ptr<Scene> scene) {
	Entity entity = scene->GetEntity(component.first.as<UUIDType>());
	auto values = component.second[YM_CAMERAVALUES];
	CameraComponent& com = entity.AddComponent<CameraComponent>();
	com.cam_speed = values[0].as<float>();
	com.Fov_y = values[1].as<float>();
	com.Aspect_ratio = values[2].as<float>();
	com.Near = values[3].as<float>();
	com.Far = values[4].as<float>();
	auto bools = component.second[YM_CAMERABOOLS];
	com.IsMainCamera = bools[0].as<bool>();

}
void DeserializeRidgidBody2D(YAML::iterator::value_type& component, std::shared_ptr<Scene> scene){
	Entity entity = scene->GetEntity(component.first.as<UUIDType>());
	RigidBody2DComponent& com = entity.AddComponent<RigidBody2DComponent>();
	com.Type = RigidBody2DComponent::BodyType{ component.second[YM_RIGIDBODY_2D_TYPE].as<int>() };
	com.FixedRotation =  component.second[YM_RIGIDBODY_2D_BOOL].as<bool>();
}
void DeserializeBox2D(YAML::iterator::value_type& component, std::shared_ptr<Scene> scene) {
	Entity entity = scene->GetEntity(component.first.as<UUIDType>());
	BoxCollider2DComponent& com = entity.AddComponent<BoxCollider2DComponent>();
	auto values = component.second[YM_BOXCOLLIDER_2D_2DVALUES];
	com.Offset = values[0].as<glm::vec2>();
	com.Size = values[1].as<glm::vec2>();
	auto values1 = component.second[YM_BOXCOLLIDER_2D_1DVALUES];
	com.Density = values1[0].as<float>();
	com.Friction = values1[1].as<float>();
	com.Restitution = values1[2].as<float>();
	com.RestitutionThreshold = values1[3].as<float>();
}
void DeserializeLight(YAML::iterator::value_type& component, std::shared_ptr<Scene> scene) {
	Entity entity = scene->GetEntity(component.first.as<UUIDType>());
	LightComponent& com = entity.AddComponent<LightComponent>();
	com.light.m_LightType = static_cast<LightType>(component.second[YM_LIGHT_TYPE].as<int>());
	com.light.m_Angle.inner = component.second[YM_LIGHT_ANGLEINNER].as<float>();
	com.light.m_Angle.outer = component.second[YM_LIGHT_ANGLEOUTER].as<float>();
	com.light.Ambient = component.second[YM_LIGHT_AMBIENT].as<glm::vec3>();
	com.light.Diffuse = component.second[YM_LIGHT_DIFFUSE].as<glm::vec3>();
	com.light.Specular = component.second[YM_LIGHT_SPECULAR].as<glm::vec3>();
	com.light.falloff = component.second[YM_LIGHT_FALLOFF].as<float>();
}
void DeserializeMesh(YAML::iterator::value_type& component, std::shared_ptr<Scene> scene) {
	Entity entity = scene->GetEntity(component.first.as<UUIDType>());
	MeshComponent& com = entity.AddComponent<MeshComponent>();
	com.handle = component.second[YM_MESH_HANDLE].as<UUIDType>();
}
void DeserializeEntity(YAML::detail::iterator_value entity, std::shared_ptr<Scene> scene) {
	std::string name{ entity[YM_NAME].as<std::string>() };
	UUIDType id = entity[YM_UUID].as<UUIDType>();
	Entity p_e = scene->CreateEntityWithUUID(id, name);
}