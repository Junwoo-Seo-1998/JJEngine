/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/23/2022
End Header-------------------------------------------------------- */
#include "Scene.h"
#include "Entity/Entity.hpp"
#include "Component/TransformComponent.h"

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

Entity Scene::CreateEntity()
{
	Entity entity{ m_Registry.create(),this };
	entity.AddComponent<TransformComponent>();
	return entity;
}

entt::registry& Scene::GetRegistry()
{
	return m_Registry;
}
