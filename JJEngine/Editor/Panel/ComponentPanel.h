#pragma once
#include <entt/entt.hpp>
class Scene;
class ComponentPanel {
	std::shared_ptr<Scene> m_scene;
	entt::entity selected_EntityHandle{ entt::null };
public:
	ComponentPanel() = default;
	ComponentPanel(std::shared_ptr<Scene> scene);

	void SetScene(std::shared_ptr<Scene> scene);
	void SetSelevted_EntityHandle(entt::entity ID);
	void OnImGuiRender();
};