#pragma once
#include <entt/entt.hpp>

#include "Core/Entity/Entity.hpp"
#include "PanelMessenger.h"
class Scene;
class ComponentPanel {
	PanelMessenger& messenger;
public:
	ComponentPanel(PanelMessenger& mg);

	void SetScene(std::weak_ptr<Scene> scene);
	void SetSelevted_EntityHandle(entt::entity ID);
	void OnImGuiRender();
private:
	void DrawComponents(Entity entity);

private:
	std::weak_ptr<Scene> m_scene;
	entt::entity selected_EntityHandle{ entt::null };
};