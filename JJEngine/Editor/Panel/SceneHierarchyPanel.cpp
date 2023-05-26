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

void SceneHierarchyPanel::DrawEntityTree(entt::entity entityID)
{
	Entity entity{ entityID, m_scene.get() };
	std::vector<UUIDType>& child = entity.GetChildrenUUID();
	ImGuiTreeNodeFlags flag{ child.empty() ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_OpenOnArrow};
	auto& name = entity.Name();
	bool opened = ImGui::TreeNodeEx(name.c_str(), flag);
	if (ImGui::IsItemClicked()) {
		setSelectedEntity(entityID);
	}
	if (opened == true) {
		for (auto& c:child) {
			DrawEntityTree(m_scene->GetEntity(c).GetEntityHandle());
		}
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
			Entity entity{ entityID, m_scene.get() };
			if (entity.GetParentUUID().is_nil() == true) {
				rootEntity.push_back(entityID);
			}
		});

	for (auto& e : rootEntity) {
		DrawEntityTree(e);
	}
	ImGui::End();
}