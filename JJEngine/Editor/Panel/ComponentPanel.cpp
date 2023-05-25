#include "ComponentPanel.h"
#include <imgui.h>
#include <Core/Application.h>
#include <Core/SceneManager.h>
#include <Core/Scene.h>
#include <Core/Entity/Entity.hpp>
ComponentPanel::ComponentPanel(std::shared_ptr<Scene> scene)
{
	SetScene(scene);
}

void ComponentPanel::SetScene(std::shared_ptr<Scene> scene)
{
	m_scene = scene;
	selected_EntityHandle = entt::null;
}

void ComponentPanel::SetSelevted_EntityHandle(entt::entity ID)
{
	selected_EntityHandle = ID;
}

void ComponentPanel::OnImGuiRender()
{
	m_scene = Application::Instance().GetSceneManager()->GetCurrentScene();
	if (m_scene == nullptr)
		return;
	ImGui::Begin("Component editting");
	if (selected_EntityHandle != entt::null) {
		Entity entity{ selected_EntityHandle, m_scene.get() };
		ImGui::Text(entity.Name().c_str());
		//auto compos = m_scene->GetRegistry().storage(selected_EntityHandle);
	}
	else {
		ImGui::Text("Default");
	}
	ImGui::End();
}
