#pragma once
#include <entt/entt.hpp>

#include "Core/Entity/Entity.hpp"
class Scene;
class ComponentPanel {
public:
	ComponentPanel() = default;

	void SetScene(std::weak_ptr<Scene> scene);
	void SetSelevted_EntityHandle(entt::entity ID);
	void OnImGuiRender();
private:
	void DrawComponents(Entity entity);

private:
	std::weak_ptr<Scene> m_scene;
	entt::entity selected_EntityHandle{ entt::null };
};