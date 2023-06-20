#pragma once
#include <memory>
#include <entt/entt.hpp>
#include "PanelMessenger.h"
class Scene;
class SceneHierarchyPanel
{
	PanelMessenger& messenger;
	void EntitySlectedFunc(unsigned entityID);
public:
	SceneHierarchyPanel(PanelMessenger& mg);
	void SetScene(std::weak_ptr<Scene> scene);
	void DrawEntityTree(entt::entity ett);
	void OnImGuiRender();
private:
	std::weak_ptr<Scene> m_scene;
	entt::entity clickedEntity{entt::null};
	bool isChildofClicked{false};
	bool isDragging{false};
	bool shouldChangeRelation{ false };
	entt::entity shouldRemoveEntity{ entt::null };
};