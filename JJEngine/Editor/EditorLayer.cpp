#include "Core/SceneSerializer.h"
#include "EditorLayer.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "glad.h"
#include "imgui.h"
#include "Core/ImGui/ImGuiRenderer.h"
#include "Core/Graphics/FrameBuffer.h"
#include "Core/Utils/Log.h"
#include "Core/Application.h"
#include "Core/Scene.h"
#include "Core/SceneManager.h"
#include "Core/ImGui/ImGuiSubWindow.h"
#include "Panel/AssetBrowserPanel.h"
#include "Core/Component/SpriteRendererComponent.h"
#include "Core/Component/TransformComponent.h"
#include "Core/Entity/Entity.hpp"
#include "Core/Graphics/Renderer/Renderer2D.h"

#include "Core/Utils/File.h"
#include "Core/Utils/Math/MatrixMath.h"


EditorLayer::~EditorLayer()
{
	editorRegistry.clear();
}

void EditorLayer::OnAttach()
{
	Log::Info("Editor Layer Added");
	active_scene = std::make_shared<Scene>();
	//for testing
	editor_viewport = FrameBuffer::CreateFrameBuffer({ 400,400,{FrameBufferFormat::RGBA, FrameBufferFormat::Depth } });

	PlayIcon = Texture::CreateTexture(File::ReadImageToTexture("Resources/Textures/UI/PlayButton.png"));
	StopIcon = Texture::CreateTexture(File::ReadImageToTexture("Resources/Textures/UI/Stop.png"));
}

void EditorLayer::OnDetach()
{

}

void EditorLayer::OnStart()
{

	component_panel.SetScene(Application::Instance().GetSceneManager()->GetCurrentScene());
	scene_hierarchy_panel.SetScene(Application::Instance().GetSceneManager()->GetCurrentScene());
	scene_hierarchy_panel.SetSlected_EntityFunc([&](entt::entity ID)->void {selected_entityID = ID; });

	entt::entity ID{ editorRegistry.create() };
	ImGuiSubWindow* temp = &editorRegistry.emplace<ImGuiSubWindow>(ID, "Asset browser");
	ABP.Set();
	temp->Push_ImGuiCommand([&]()->void {ABP.OnImGuiRender(); });


	auto test_texture = Texture::CreateTexture(File::ReadImageToTexture("Resources/Textures/test.jpg"));
	Entity entity=active_scene->CreateEntity();
	entity.AddComponent<SpriteRendererComponent>(test_texture);
}

void EditorLayer::OnUpdate()
{
	//get something from scene
}

void EditorLayer::OnPreRender()
{
	editor_viewport->Bind(true);
	glViewport(0, 0, 400, 400);
}

void EditorLayer::OnRender()
{
	auto& reg = active_scene->GetRegistry();
	glm::mat4 viewProj = glm::perspective(400.f, 1.f, 0.001f, 100.f);
	viewProj *=MatrixMath::BuildCameraMatrixWithDirection({ 0,0,1 }, { 0,0,-1.f });

	Renderer2D::BeginScene(viewProj);

	auto view = reg.view<SpriteRendererComponent>();

	for (auto e: view)
	{
		Entity entity{ e, active_scene.get() };
		Renderer2D::DrawQuad(entity.GetWorldSpaceTransformMatrix(), { 1,1,1,1 });
	}
	Renderer2D::EndScene();
}

void EditorLayer::OnPostRender()
{
	editor_viewport->UnBind();
}

void EditorLayer::OnImGuiRender()
{
	ImGuiRenderer::Instance()->GuiDrawDockSpaceBegin();
	if (ImGui::BeginMenuBar()) // testing for now
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save scene")) { 
				SceneSerializer see_real( Application::Instance().GetSceneManager()->GetCurrentScene() );
				see_real.Serialize("./Resources/Scenes/testScene.scn");
			}
			if (ImGui::MenuItem("Load test scene")) {
				Application::Instance().GetSceneManager()->GetCurrentScene()->GetRegistry().clear();
				SceneSerializer see_real(Application::Instance().GetSceneManager()->GetCurrentScene());
				if (see_real.Deserialize("./Resources/Scenes/testScene.scn") == false) Log::Error("Fail to deserialize testScene");
			}
			if (ImGui::MenuItem("Load test22 scene")) {
				Application::Instance().GetSceneManager()->GetCurrentScene()->GetRegistry().clear();
				SceneSerializer see_real(Application::Instance().GetSceneManager()->GetCurrentScene());
				if(see_real.Deserialize("./Resources/Scenes/testScene22.scn") == false)Log::Error("Fail to deserialize testScene");
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	auto panels = editorRegistry.view<ImGuiSubWindow>();
	for (auto& ID:panels) {
		panels.get<ImGuiSubWindow>(ID).Update();
	}

	ImGui::Begin("viewport");
	ImGui::Image((void*)editor_viewport->GetColorTexture(0)->GetTextureID(), { 400, 400 }, { 0,1 }, { 1,0 });
	ImGui::End();


	scene_hierarchy_panel.OnImGuiRender();
	component_panel.SetSelevted_EntityHandle(selected_entityID);
	component_panel.OnImGuiRender();

	DrawToolBar();

	ImGuiRenderer::Instance()->GuiDrawDockSpaceEnd();
}

void EditorLayer::DrawToolBar()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.f,2.f });
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2{ 0.f,0.f });

	auto& color = ImGui::GetStyle().Colors;
	const auto& buttonHovered = color[ImGuiCol_ButtonHovered];
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f });
	const auto& buttonActive = color[ImGuiCol_ButtonActive];
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ buttonActive.x, buttonActive.y, buttonActive.z,0 });

	ImGui::Begin("##toolbar", nullptr,
		ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	std::shared_ptr<Texture> TextureToUse = scene_state == SceneState::Play ? PlayIcon : StopIcon;
	float buttonSize = ImGui::GetWindowHeight() - 4.0f;
	ImGui::SameLine((ImGui::GetContentRegionMax().x * 0.5f) - (buttonSize * 0.5f));
	if (ImGui::ImageButton((ImTextureID)TextureToUse->GetTextureID(), 
		ImVec2{ buttonSize, buttonSize }, ImVec2{ 0.f, 0.f }, ImVec2{ 1.f, 1.f }, 0))
	{
		if (scene_state == SceneState::Edit)
		{
			OnScenePlay();
		}
		else if (scene_state == SceneState::Play)
		{
			OnSceneStop();
		}
	}
	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);
	ImGui::End();
}

void EditorLayer::OnScenePlay()
{
	scene_state = SceneState::Play;
}

void EditorLayer::OnSceneStop()
{
	scene_state = SceneState::Edit;
}
