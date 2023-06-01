#include "SceneHierarchyPanel.h"
#include "imgui.h"
#include "Core/Application.h"
#include "Core/Scene.h"
#include "Core/SceneManager.h"
#include "Core/Entity/Entity.hpp"
#include "Core/Utils/Log.h"
#include "Core/Utils/Assert.h"
void SceneHierarchyPanel::SetSlected_EntityFunc(std::function<void(entt::entity)> func)
{
	setSelectedEntity = func;
}

void SceneHierarchyPanel::SetScene(std::weak_ptr<Scene> scene)
{
	m_scene = scene;
}

void SceneHierarchyPanel::DrawEntityTree(entt::entity entityID)
{
	auto scene = m_scene.lock();
	ENGINE_ASSERT(scene, "check the original scene was deleted!!");

	Entity entity{ entityID, scene.get() };
	std::vector<UUIDType>& child = entity.GetChildrenUUID();
	ImGuiTreeNodeFlags flag{ child.empty() ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_OpenOnArrow};
	auto& name = entity.Name();
	bool opened = ImGui::TreeNodeEx(name.c_str(), flag);
	if (ImGui::IsItemClicked()) {
		setSelectedEntity(entityID);
	}
	if (opened == true) {
		for (auto& c:child) {
			DrawEntityTree(scene->GetEntity(c).GetEntityHandle());
		}
		ImGui::TreePop();
	}
}


void SceneHierarchyPanel::OnImGuiRender()
{
	auto scene = m_scene.lock();
	ENGINE_ASSERT(scene, "check the original scene was deleted!!");

	ImGui::Begin("Scene Hierarchy");
	std::vector < entt::entity > rootEntity{};
	scene->m_Registry.each([&](auto entityID)
		{
			Entity entity{ entityID, scene.get() };
			if (entity.GetParentUUID().is_nil() == true) {
				rootEntity.push_back(entityID);
			}
		});

	for (auto& e : rootEntity) {
		DrawEntityTree(e);
	}
	ImGui::End();
}