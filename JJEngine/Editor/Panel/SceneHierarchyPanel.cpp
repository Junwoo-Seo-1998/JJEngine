#include "SceneHierarchyPanel.h"
#include "imgui.h"
#include "Core/Application.h"
#include "Core/Scene.h"
#include "Core/SceneManager.h"
#include "Core/Entity/Entity.hpp"
#include "Core/Utils/Log.h"
#include "Core/Utils/Assert.h"
#include "Core/Component/CameraComponent.h"

void SceneHierarchyPanel::SetSlected_EntityFunc(std::function<void(entt::entity)> func)
{
	setSelectedEntity = func;
}

void SceneHierarchyPanel::SetScene(std::weak_ptr<Scene> scene)
{
	m_scene = scene;
	clickedEntity = entt::null;
}

void SceneHierarchyPanel::DrawEntityTree(entt::entity entityID)
{
	auto scene = m_scene.lock();
	ENGINE_ASSERT(scene, "check the original scene was deleted!!");

	Entity entity{ entityID, scene.get() };
	std::vector<UUIDType>& child = entity.GetChildrenUUID();
	ImGuiTreeNodeFlags flag{ child.empty() ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_OpenOnArrow};
	auto name = entity.Name()+"##"+ std::to_string((unsigned)entityID);
	bool opened = ImGui::TreeNodeEx(name.c_str(), flag);
	bool isHovered{ ImGui::IsItemHovered()};
	if (ImGui::IsItemClicked()) {
		setSelectedEntity(entityID);
	}
	if (ImGui::IsItemActive() == true) {
		//Log::Info("EID" + std::to_string((int)entityID));
		// Dragging feedback
		clickedEntity = entityID;
		isDragging = true;
	}
	if (clickedEntity == entityID) isChildofClicked = true;
	if (shouldChangeRelation == true 
		&& isHovered == true) {
		//Log::Info("Hovered EID" + std::to_string((int)entityID));
		if (isChildofClicked == false) {
			Entity ChildEntity{ clickedEntity, scene.get() };
			ChildEntity.SetParent(entity);
		}
		shouldChangeRelation = false;
	}

	if (opened == true) {
		for (auto& c:child) {
			DrawEntityTree(scene->GetEntity(c).GetEntityHandle());
		}
		ImGui::TreePop();
	}
	if (clickedEntity == entityID) isChildofClicked = false;



	if (isHovered == true 
		&& ImGui::IsMouseReleased(ImGuiMouseButton_Right) == true) {
		shouldRemoveEntity = entityID;
	}
}


void SceneHierarchyPanel::OnImGuiRender()
{
	auto scene = m_scene.lock();
	ENGINE_ASSERT(scene, "check the original scene was deleted!!");

	if (ImGui::BeginMenuBar() == true) {
		if (ImGui::BeginMenu("Create") == true) {
			if (ImGui::MenuItem("Create empty entity") == true) {
				Entity child = scene->CreateEntity(); // temp
				if (clickedEntity != entt::null) {
					Entity parent{ clickedEntity,scene.get() };
					child.SetParent(parent);
				}
			}
			if (ImGui::MenuItem("Create Camera") == true) {
				Entity entity = scene->CreateEntity("Camera");
				entity.AddComponent<CameraComponent>();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::Begin("Scene Hierarchy");
	ImGui::Text("Scene name: %s", scene->m_scene_name.c_str());

	ImGui::Separator();


	std::vector < entt::entity > rootEntity{};
	scene->m_Registry.each([&](auto entityID)
		{
			Entity entity{ entityID, scene.get() };
			if (entity.GetParentUUID().is_nil() == true) {
				rootEntity.push_back(entityID);
			}
		});

	bool opened = ImGui::TreeNodeEx("Scene", ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen);
	if (ImGui::IsItemClicked()) {
		setSelectedEntity(entt::null);
		clickedEntity = entt::null;
	}
	if (opened == true) {
		for (auto& e : rootEntity) {
			DrawEntityTree(e);
		}
		ImGui::TreePop();
	}

	if (shouldChangeRelation == true) {//clear parent
		Entity ChildEntity{ clickedEntity, scene.get() };
		//ChildEntity.SetParent(); // nead ClearParent function
		Log::Info("Clear parent");
		shouldChangeRelation = false;
	}

	if (isDragging == true && ImGui::IsMouseReleased(ImGuiMouseButton_Left) == true) {
		//Log::Info("Drag end");
		isDragging = false;
		shouldChangeRelation = true;
	}

	if (shouldRemoveEntity != entt::null && ImGui::BeginPopupContextWindow("Entity option"))
	{
		if (ImGui::Button("Remove")) { // will change into event-driven
			scene->DestroyEntity(Entity{ shouldRemoveEntity, scene.get() });
			setSelectedEntity(entt::null);//jun: don't forget
			shouldRemoveEntity = entt::null;
			clickedEntity = entt::null;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	else {
		shouldRemoveEntity = entt::null;
	}

	ImGui::Separator();
	ImGui::End();
}