#include "SceneHierarchyPanel.h"
#include "imgui.h"
#include "Core/Application.h"
#include "Core/Scene.h"
#include "Core/SceneManager.h"
#include "Core/Entity/Entity.hpp"
#include "Core/Utils/Log.h"

SceneHierarchyPanel::SceneHierarchyPanel(std::shared_ptr<Scene> scene)
{
	//since might do other things in future.
	SetScene(scene);
}

void SceneHierarchyPanel::SetSlected_EntityFunc(std::function<void(entt::entity)> func)
{
	setSelectedEntity = func;
}

void SceneHierarchyPanel::SetScene(std::shared_ptr<Scene> scene)
{
	m_scene = scene;
}

void SceneHierarchyPanel::DrawEntityTree(entt::entity& entityID)
{
	ImGuiTreeNodeFlags flag{ImGuiTreeNodeFlags_OpenOnArrow};
	Entity entity{ entityID, m_scene.get() };
	auto& name = entity.Name();
	bool opened = ImGui::TreeNodeEx(name.c_str(), flag);
	if (ImGui::IsItemClicked()) {
		setSelectedEntity(entityID);
		//Log::Info(name);
	}
	if (opened == true) {
		ImGui::Text(name.c_str());
		ImGui::TreePop();
	}
}


void SceneHierarchyPanel::OnImGuiRender()
{
	m_scene = Application::Instance().GetSceneManager()->GetCurrentScene();
	if(m_scene == nullptr)
		return;
	ImGui::Begin("Scene Hierarchy");
	std::vector < entt::entity > rootEntity{};
	m_scene->m_Registry.each([&](auto entityID)
		{
			rootEntity.push_back(entityID);
		});

	for (auto& e : rootEntity) {
		DrawEntityTree(e);
	}
	ImGui::End();
}