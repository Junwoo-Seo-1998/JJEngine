#pragma once
#include <memory>
#include <entt/entt.hpp>
class Scene;
class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(std::shared_ptr<Scene> scene);
	void SetSlected_EntityFunc(std::function<void(entt::entity)> func);
	void SetScene(std::shared_ptr<Scene> scene);
	void DrawEntityTree(entt::entity ett);
	void OnImGuiRender();
private:
	std::shared_ptr<Scene> m_scene;
	std::function<void(entt::entity)> setSelectedEntity;
};