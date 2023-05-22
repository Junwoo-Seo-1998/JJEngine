/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/23/2022
End Header-------------------------------------------------------- */
#include "Scene.h"
#include "Entity/Entity.hpp"
#include "Component/TransformComponent.h"
#include "Component/NameComponent.h"
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
	entity.AddComponent<TransformComponent>();
	if (!name.empty())
		entity.AddComponent<NameComponent>(name);
	return entity;
}

entt::registry& Scene::GetRegistry()
{
	return m_Registry;
}
