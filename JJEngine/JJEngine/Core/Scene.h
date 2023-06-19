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
#include <memory>
#include <glm/glm.hpp>

class EditorCamera;
class Entity;
class SceneHierarchyPanel;
class b2World;
class VertexArray;
class FrameBuffer;
class Mesh;
using EntityMap = std::unordered_map<UUIDType, Entity>;

class Scene
{
	friend class Entity;
	friend class SceneHierarchyPanel;
	friend class SceneSerializer;
	//static function

	static constexpr unsigned int MAX_SHADOW_BUFFER = 6;
	std::shared_ptr<VertexArray> renderer_vao;
	std::shared_ptr<FrameBuffer> g_buffer;
	std::shared_ptr<FrameBuffer> shadow_buffer; 
	std::shared_ptr <Mesh> FSQ;

public:
	static std::shared_ptr<Scene> Copy(std::shared_ptr<Scene> toCopy);
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

	void RenderScene(const glm::mat4& viewProj, const glm::vec3& cameraPos);

	//editor only
	void UpdateEditor(EditorCamera& camera);
	void RenderEntityID(EditorCamera& camera);
	//runtime only
	void StartRuntime();
	void UpdateRuntime();
	void StopRuntime();

	Entity CreateEntity(const std::string& name = "No Name");
	Entity CreateEntityWithUUID(UUIDType uuid, const std::string& name = "No Name", bool sort = true);

	void DestroyEntity(Entity entity, bool excludeChildren = false);

	Entity GetEntity(UUIDType uuid) const;
	Entity TryGetEntity(UUIDType uuid) const;
	Entity TryGetEntity(const std::string& name);
	Entity GetMainCameraEntity();

	std::string GetSceneName() const;
	std::filesystem::path GetScenePath() const;
	void SetScenePath(std::filesystem::path);

	entt::registry& GetRegistry();
	const EntityMap& GetEntityMap() const;

	//update all viewport size of entities that have camera components
	void ResizeViewport(unsigned int width, unsigned int height);


	/*template<typename ComponentType>
	void CopyComponentIfExists(entt::entity dst, entt::registry& dstReg, entt::entity src)
	{
		if (m_Registry.any_of<ComponentType>(src))
		{
			auto& srcComponent = m_Registry.get<ComponentType>(src);
			dstReg.emplace_or_replace<ComponentType>(dst, srcComponent);
		}
	}

	template<typename ComponentType>
	static void CopyComponentIfExistsFromScene(std::shared_ptr<Scene> dstScene, Entity dst, std::shared_ptr<Scene> srcScene, Entity src)
	{
		srcScene->CopyComponentIfExists<ComponentType>(dst.GetEntityHandle(), dstScene->m_Registry, src.GetEntityHandle());
	}*/
private:
	void StartPhysics2D();
	void StopPhysics2D();

protected:
	void SortEntityMap();
	void DestroyEntityHelper(Entity entity, bool excludeChildren = false, bool first = true);

	unsigned int m_ViewportWidth = 0, m_ViewportHeight = 0;

	std::string m_scene_name;
	std::filesystem::path scenePath;
	entt::registry m_Registry;

	//to register entities based on uuid
	EntityMap m_entity_map;

	std::unique_ptr<b2World> m_2DWorld;
	
};