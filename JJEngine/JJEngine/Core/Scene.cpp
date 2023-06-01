/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/23/2022
End Header-------------------------------------------------------- */
#include "Scene.h"

#include "Component/CameraComponent.h"
#include "Component/SpriteRendererComponent.h"
#include "Entity/Entity.hpp"
#include "Component/TransformComponent.h"
#include "Graphics/Renderer/Renderer2D.h"
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

void Scene::UpdateEditor(EditorCamera& camera)
{
	Renderer2D::BeginScene(camera);
	auto view = m_Registry.view<SpriteRendererComponent>();
	for (auto e : view)
	{
		Entity entity{ e, this };
		auto& spriteComp = entity.GetComponent<SpriteRendererComponent>();
		if (spriteComp.texture)
			Renderer2D::DrawQuad(entity.GetWorldSpaceTransformMatrix(), spriteComp.texture, spriteComp.color);
		else
			Renderer2D::DrawQuad(entity.GetWorldSpaceTransformMatrix(), spriteComp.color);
	}
	Renderer2D::EndScene();
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

Entity Scene::CreateEntityWithUUID(UUIDType uuid, const std::string& name, bool sort)
{
	auto entity = Entity{ m_Registry.create(), this };

	auto& uuidComponent = entity.AddComponent<UUIDComponent>();
	uuidComponent.UUID = uuid;

	entity.AddComponent<TransformComponent>();
	if (!name.empty())
		entity.AddComponent<NameComponent>(name);

	entity.AddComponent<RelationshipComponent>();
	ENGINE_ASSERT(m_entity_map.find(uuidComponent.UUID) == m_entity_map.end(), "There is an Entity already with same uuid");
	m_entity_map[uuidComponent.UUID] = entity;

	//for editor view
	if (sort)
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

std::string Scene::GetSceneName() const {
	return m_scene_name;
}

std::filesystem::path Scene::GetScenePath() const {
	return scenePath;
}
void Scene::SetScenePath(std::filesystem::path path) {
	scenePath = path;
}

entt::registry& Scene::GetRegistry()
{
	return m_Registry;
}

const EntityMap& Scene::GetEntityMap() const
{
	return m_entity_map;
}

void Scene::ResizeViewport(unsigned width, unsigned height)
{
	m_ViewportWidth = width;
	m_ViewportHeight = height;

	//resize non fixed aspect ratio
	auto view = m_Registry.view<CameraComponent>();
	for (auto entity : view)
	{
		auto& cameraComponent = view.get<CameraComponent>(entity);
		cameraComponent.aspect_ratio = width / height;
	}
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
