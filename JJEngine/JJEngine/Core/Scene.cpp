/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/23/2022
End Header-------------------------------------------------------- */
#include "Scene.h"
#include "Entity/Entity.hpp"
#include "Component/TransformComponent.h"
#include "Utils/Assert.h"
#include "Utils/UUIDGenerator.h"

Scene::Scene()
	:m_scene_name("unnamed-scene")
{
}

Scene::Scene(const std::string& scene_name)
	:m_scene_name(scene_name)
{
}

Scene::~Scene()
{
}

void Scene::Awake()
{
}

void Scene::OnEnable()
{
}

void Scene::Start()
{
}

void Scene::Update()
{
}

void Scene::PostUpdate()
{
}

void Scene::OnDisable()
{
}

void Scene::OnDestroy()
{
}

Entity Scene::CreateEntity(const std::string& name)
{
	Entity entity{ m_Registry.create(),this };

	auto& uuidComponent = entity.AddComponent<UUIDComponent>();
	uuidComponent.UUID = UUIDGenerator::Generate();

	entity.AddComponent<TransformComponent>();
	if(!name.empty())
		entity.AddComponent<NameComponent>(name);

	entity.AddComponent<RelationshipComponent>();
	m_entity_map[uuidComponent.UUID] = entity;

	//for editor view
	SortEntityMap();

	return entity;
}

Entity Scene::GetEntity(UUIDType uuid) const
{
	ENGINE_ASSERT(m_entity_map.find(uuid) != m_entity_map.end(), "There is no entity with given UUID");
	return m_entity_map.at(uuid);
}

Entity Scene::TryGetEntity(UUIDType uuid) const
{
	if(const auto found = m_entity_map.find(uuid); found != m_entity_map.end())
		return found->second;
	return Entity{};
}

Entity Scene::TryGetEntity(const std::string& name)
{
	const auto entities = m_Registry.view<NameComponent>();
	for(auto entity : entities)
	{
		if (entities.get<NameComponent>(entity).Name == name)
			return { entity, this };
	}
	return {};
}

entt::registry& Scene::GetRegistry()
{
	return m_Registry;
}

void Scene::SortEntityMap()
{
	m_Registry.sort<UUIDComponent>([&](const auto left, const auto right)
	{
		auto leftEntity = m_entity_map.find(left.UUID);
		auto rightEntity = m_entity_map.find(right.UUID);
		return leftEntity->second < rightEntity->second;
	});
}
