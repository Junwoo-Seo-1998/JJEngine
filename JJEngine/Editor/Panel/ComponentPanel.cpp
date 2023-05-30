#include "ComponentPanel.h"
#include <imgui.h>
#include <Core/Application.h>
#include <Core/SceneManager.h>
#include <Core/Scene.h>
#include <Core/Entity/Entity.hpp>
#include <array>
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
	ImGui::Begin("Component Editing");
	if (selected_EntityHandle != entt::null) {
		Entity entity{ selected_EntityHandle, m_scene.get() };
		DrawComponents(entity);
		//auto compos = m_scene->GetRegistry().storage(selected_EntityHandle);
	}
	else {
		ImGui::Text("Please Select Entity");
	}
	ImGui::End();
}

void ComponentPanel::DrawComponents(Entity entity)
{
	{
		std::array<char, 256> inputText{ 0, };
		auto& name = entity.Name();
		std::copy(name.begin(), name.end(), inputText.data());
		ImGui::Text(entity.Name().c_str());
		if(ImGui::InputText("Name", inputText.data(), inputText.size()))
		{
			if(!entity.HasComponent<NameComponent>())
			{
				entity.AddComponent<NameComponent>(inputText.data());
			}
			else
			{
				name = inputText.data();
			}
		}
	}
}
