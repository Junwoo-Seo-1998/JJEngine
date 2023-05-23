#include "SceneHierarchyPanel.h"
#include "imgui.h"
#include "Core/Application.h"
#include "Core/Scene.h"
#include "Core/SceneManager.h"
#include "Core/Component/NameComponent.h"
#include "Core/Entity/Entity.hpp"

SceneHierarchyPanel::SceneHierarchyPanel(std::shared_ptr<Scene> scene)
{
	//since might do other things in future.
	SetScene(scene);
}

void SceneHierarchyPanel::SetScene(std::shared_ptr<Scene> scene)
{
	m_scene = scene;
}

void SceneHierarchyPanel::OnImGuiRender()
{
	m_scene = Application::Instance().GetSceneManager()->GetCurrentScene();
	if(m_scene == nullptr)
		return;
	ImGui::Begin("Scene Hierarchy");
	m_scene->m_Registry.each([&](auto entityID)
		{
			Entity entity{ entityID, m_scene.get() };
			auto& name = entity.GetComponent<NameComponent>();
			ImGui::Text("%s", name.Name.c_str());
		});
	ImGui::End();
}
