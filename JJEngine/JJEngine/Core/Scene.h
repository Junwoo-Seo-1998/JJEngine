/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/17/2022
End Header-------------------------------------------------------- */
#pragma once
#include <string>
#include <entt/entt.hpp>
#include "Type.h"

class Entity;
class SceneHierarchyPanel;
using EntityMap = std::unordered_map<UUIDType, Entity>;

class Scene
{
	friend class Entity;
	friend class SceneHierarchyPanel;
public:
	Scene();
	Scene(const std::string& scene_name);
	virtual ~Scene();

	virtual void Awake();
	virtual void OnEnable();
	virtual void Start();
	virtual void Update();
	virtual void PostUpdate();
	virtual void OnDisable();
	virtual void OnDestroy();

	Entity CreateEntity(const std::string& name = {});

	Entity GetEntity(UUIDType uuid) const;
	Entity TryGetEntity(UUIDType uuid) const;
	Entity TryGetEntity(const std::string& name) const;


	entt::registry& GetRegistry();
protected:
	void SortEntityMap();

	std::string m_scene_name;
	entt::registry m_Registry;

	//to register entities based on uuid
	EntityMap m_entity_map;
	
};