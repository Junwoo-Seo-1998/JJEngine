/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/17/2022
End Header-------------------------------------------------------- */
#pragma once
#include <string>
#include <entt/entt.hpp>
#include <filesystem>
#include "Type.h"

class EditorCamera;
class Entity;
class SceneHierarchyPanel;
using EntityMap = std::unordered_map<UUIDType, Entity>;

class Scene
{
	friend class Entity;
	friend class SceneHierarchyPanel;
	friend class SceneSerializer;
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

	//editor only
	void UpdateEditor(EditorCamera& camera);



	Entity CreateEntity(const std::string& name = {});
	Entity CreateEntityWithUUID(UUIDType uuid, const std::string& name = {}, bool sort = true);

	Entity GetEntity(UUIDType uuid) const;
	Entity TryGetEntity(UUIDType uuid) const;
	Entity TryGetEntity(const std::string& name);

	std::string GetSceneName() const;
	std::filesystem::path GetScenePath() const;
	void SetScenePath(std::filesystem::path);

	entt::registry& GetRegistry();
	const EntityMap& GetEntityMap() const;

	//update all viewport size of entities that have camera components
	void ResizeViewport(unsigned int width, unsigned int height);
protected:
	void SortEntityMap();

	unsigned int m_ViewportWidth = 0, m_ViewportHeight = 0;

	std::string m_scene_name;
	std::filesystem::path scenePath;
	entt::registry m_Registry;

	//to register entities based on uuid
	EntityMap m_entity_map;


	
};