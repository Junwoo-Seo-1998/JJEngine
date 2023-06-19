#include "ComponentPanel.h"
#include <imgui.h>
#include <Core/Application.h>
#include <Core/SceneManager.h>
#include <Core/Scene.h>
#include <Core/Entity/Entity.hpp>
#include <array>
#include <glm/gtc/type_ptr.hpp>

#include "imgui_internal.h"
#include "Core/Component/BoxCollider2DComponent.h"
#include "Core/Component/RigidBody2DComponent.h"
#include "Core/Component/ScriptComponent.h"
#include "Core/Component/SpriteRendererComponent.h"

#include "Core/Component/LightComponent.h"
#include "Core/Component/MaterialComponent.h"

#include "Core/Utils/Assert.h"

#include "Core/Script/ScriptEngine.h"
#include <format>
#include <Core/Graphics/Mesh.h>
static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f,lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f,0.1f,0.15f ,1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f,0.2f,0.2f ,1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f,0.1f,0.15f ,1.0f });

	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;
	ImGui::PopFont();

	ImGui::PopStyleColor(3);


	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f,0.7f,0.2f ,1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f,0.8f,0.2f ,1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f,0.7f,0.2f ,1.0f });

	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;
	ImGui::PopStyleColor(3);
	ImGui::PopFont();

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();


	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.25f,0.8f ,1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.35f,0.9f ,1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f,0.25f,0.8f ,1.0f });

	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;
	ImGui::PopStyleColor(3);
	ImGui::PopFont();

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

template<typename T, typename UIFunction>
static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
{
	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed
		| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
	if (entity.HasComponent<T>())
	{
		auto& component = entity.GetComponent<T>();
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		ImGui::PopStyleVar();


		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
		if (ImGui::Button("+", ImVec2{ lineHeight,lineHeight }))
		{
			ImGui::OpenPopup("Component Settings");
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("Component Settings"))
		{
			if (ImGui::MenuItem("Remove Component"))
				removeComponent = true;
			ImGui::EndPopup();
		}

		if (open)
		{
			uiFunction(component);
			ImGui::TreePop();
		}

		if (removeComponent)
			entity.RemoveComponent<T>();
	}
}

void ComponentPanel::SetScene(std::weak_ptr<Scene> scene)
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
	auto scene = m_scene.lock();
	ENGINE_ASSERT(scene, "check the original scene was deleted!!");

	ImGui::Begin("Component Editing");
	if (selected_EntityHandle != entt::null) {
		Entity entity{ selected_EntityHandle, scene.get() };
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
		ImGui::Text(std::format("UUID : {}", to_string(entity.GetUUID())).c_str());

		std::array<char, 256> inputText{ 0, };
		auto& name = entity.Name();
		std::copy(name.begin(), name.end(), inputText.data());
		if(ImGui::InputText("##Name", inputText.data(), inputText.size()))
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

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("Add Component");

		if (ImGui::BeginPopup("Add Component"))
		{
			if (!entity.HasComponent<ScriptComponent>())
			{
				if (ImGui::MenuItem("Script Component"))
				{
					entity.AddComponent<ScriptComponent>();
					ImGui::CloseCurrentPopup();
				}
			}


			if(!entity.HasComponent<SpriteRendererComponent>())
			{
				if (ImGui::MenuItem("Sprite Renderer"))
				{
					entity.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if(!entity.HasComponent<RigidBody2DComponent>())
			{
				if (ImGui::MenuItem("RigidBody2D"))
				{
					entity.AddComponent<RigidBody2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<BoxCollider2DComponent>())
			{
				if (ImGui::MenuItem("BoxCollider2D"))
				{
					entity.AddComponent<BoxCollider2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<LightComponent>())
			{
				if (ImGui::MenuItem("Light"))
				{
					entity.AddComponent<LightComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<MaterialComponent>()) //??
			{
				if (ImGui::MenuItem("Material"))
				{
					entity.AddComponent<MaterialComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<Model>())
			{
				if (ImGui::MenuItem("Model"))
				{
					Model& model = entity.AddComponent<Model>();
					model.GetMeshes().push_back(std::make_shared<Mesh>(Mesh::CreateSphere(10, 10, 10, {})));
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			DrawVec3Control("Position", component.Position);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale, 1.f);
		});


		DrawComponent<ScriptComponent>("C# Script", entity, [](auto& component)
		{
			bool scriptClassExists = Script::ScriptEngine::EntityClassExists(component.Name);

			static char buffer[64];
			std::strcpy(buffer, component.Name.c_str());

			if (!scriptClassExists)
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.9f, 0.2f, 0.3f,1.f });

			if(ImGui::InputText("Class", buffer, sizeof(buffer)))
			{
				component.Name = buffer;
			}

			if (!scriptClassExists)
				ImGui::PopStyleColor();
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
		});

		DrawComponent<LightComponent>("Light", entity, [](auto& component)
			{
			});

		DrawComponent<MaterialComponent>("Material", entity, [](auto& component)
			{
			});

		DrawComponent<Model>("Model", entity, [](auto& component)
			{
			});

		DrawComponent<RigidBody2DComponent>("RigidBody 2D", entity, [](auto& component)
		{
			const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
			const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];
			if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
					if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
					{
						currentBodyTypeString = bodyTypeStrings[i];
						component.Type = (RigidBody2DComponent::BodyType)i;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
		});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component)
		{
			ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
			ImGui::DragFloat2("Size", glm::value_ptr(component.Size));
			ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
		});
	}
}
