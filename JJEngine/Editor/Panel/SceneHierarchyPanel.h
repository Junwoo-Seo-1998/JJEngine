#pragma once
#include <memory>
#include <entt/entt.hpp>
class Scene;
class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel() = default;
	void SetSlected_EntityFunc(std::function<void(entt::entity)> func);
	void SetScene(std::weak_ptr<Scene> scene);
	void DrawEntityTree(entt::entity ett);
	void OnImGuiRender();
private:
	std::weak_ptr<Scene> m_scene;
	std::function<void(entt::entity)> setSelectedEntity;
	entt::entity clickedEntity{entt::null};
	bool isChildofClicked{false};
	bool isDragging{false};
	bool shouldChangeRelation{ false };
};