#include "Core/SceneSerializer.h"
#include "EditorLayer.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "glad.h"
#include "imgui.h"
#include "TestScene.h"
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
	editor_camera = EditorCamera{ 45.f, 1.f, 0.01f, 100.f };
	active_scene = std::make_shared<TestScene>("Test");
	active_scene->Awake();
	active_scene->Start();
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
	component_panel.SetScene(active_scene);
	scene_hierarchy_panel.SetScene(active_scene);
	scene_hierarchy_panel.SetSlected_EntityFunc([&](entt::entity ID)->void {selected_entityID = ID; });


	entt::entity ID{ editorRegistry.create() };
	ImGuiSubWindow* temp = &editorRegistry.emplace<ImGuiSubWindow>(ID, "Asset browser");
	ABP.Set();
	ABP.SetSelectedFileFunc([&](std::filesystem::path str)->void {shouldOpenFile = str; });
	temp->Push_ImGuiCommand([&]()->void {ABP.OnImGuiRender(); });
}

void EditorLayer::OnUpdate()
{
	editor_camera.OnUpdate();


	if (shouldOpenFile.empty() == false) {
		SetNewScene(std::make_shared<Scene>(shouldOpenFile.filename().string()));
		SceneSerializer see_real(active_scene);
		if (see_real.Deserialize(shouldOpenFile.string()) == false) Log::Error("Fail to deserialize " + shouldOpenFile.filename().string());
		shouldOpenFile.clear();
	}
}

void EditorLayer::OnPreRender()
{
	editor_viewport->Bind(true);
	glViewport(0, 0, 400, 400);
}

void EditorLayer::OnRender()
{
	if (!active_scene)
		return;
	active_scene->UpdateEditor(editor_camera);
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
				SceneSerializer see_real(active_scene);
				see_real.Serialize("./Resources/Scenes/testScene.scn");
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	auto panels = editorRegistry.view<ImGuiSubWindow>();
	for (auto& ID : panels) {
		panels.get<ImGuiSubWindow>(ID).Update();
	}

	ImGui::Begin("viewport");
	ImGui::Image((void*)editor_viewport->GetColorTexture(0)->GetTextureID(), { 400, 400 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
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

	std::shared_ptr<Texture> TextureToUse = scene_state == SceneState::Edit ? PlayIcon : StopIcon;
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

void EditorLayer::SetNewScene(std::shared_ptr<Scene> new_scene) {
	selected_entityID = entt::null;
	active_scene = new_scene;
	component_panel.SetScene(active_scene);
	scene_hierarchy_panel.SetScene(active_scene);
	active_scene->Awake();
	active_scene->Start();
}
