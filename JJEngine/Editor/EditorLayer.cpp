#include "Core/SceneSerializer.h"
#include "EditorLayer.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glad.h"
#include "imgui.h"
#include "ImGuizmo.h"
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

#include "Core/Event/WindowEvent.h"
#include "Core/Event/EventManager.h"
#include "Core/Input/Input.h"

EditorLayer::~EditorLayer()
{
	editorRegistry.clear();
}

void EditorLayer::OnAttach()
{
	Log::Info("Editor Layer Added");
	editor_camera = EditorCamera{ 45.f, 1.f, 0.01f, 100.f };
	SetNewScene(std::make_shared<TestScene>("Test"));
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
	switch (scene_state)
	{
	case SceneState::Edit:
		editor_camera.OnUpdate();
		SelectGuizmo();
		break;
	case SceneState::Play:
		//place holder
		break;
	}

	
	if (shouldOpenFile.extension().string() == ".scn") {
		//TODO: ask save now scene
		SetNewScene(std::make_shared<Scene>(shouldOpenFile.filename().string()));
		SceneSerializer see_real(active_scene);
		active_scene->SetScenePath(shouldOpenFile);
		if (see_real.Deserialize(shouldOpenFile.string()) == false) Log::Error("Fail to deserialize Scene: " + shouldOpenFile.filename().string());
	}
	shouldOpenFile.clear();
}

void EditorLayer::OnPreRender()
{
	auto spec = editor_viewport->GetSpecification();
	const bool viewportSizeChanged = (spec.Width != viewport_size.x || spec.Height != viewport_size.y);
	if (viewportSizeChanged && viewport_size.x > 0.0f && viewport_size.y > 0.0f)
	{
		editor_camera.SetViewportSize((unsigned)viewport_size.x, (unsigned)viewport_size.y);
		active_scene->ResizeViewport((unsigned)viewport_size.x, (unsigned)viewport_size.y);
		editor_viewport->Resize((unsigned)viewport_size.x, (unsigned)viewport_size.y);
	}
	editor_viewport->Bind(true);
	glViewport(0, 0, (int)spec.Width, (int)spec.Height);
}

void EditorLayer::OnRender()
{
	if (!active_scene)
		return;
	switch (scene_state)
	{
	case SceneState::Edit:
		active_scene->UpdateEditor(editor_camera);
		break;
	case SceneState::Play:
		active_scene->UpdateRuntime();
		break;
	}
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
			if (ImGui::MenuItem("Make scene")) {
				//TODO: name
			}
			if (ImGui::MenuItem("Save scene")) {
				SceneSerializer see_real(active_scene);
				if (active_scene->GetScenePath().extension().string() == ".scn") {
					see_real.Serialize(active_scene->GetScenePath().string());
				}
				else {
					see_real.Serialize("./Resources/Scenes/" + active_scene->GetSceneName() + ".scn");
				}
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
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	viewport_size = { viewportPanelSize.x, viewportPanelSize.y };
	ImGui::Image((void*)editor_viewport->GetColorTexture(0)->GetTextureID(), viewportPanelSize, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	if(scene_state==SceneState::Edit)
		DrawGuizmo(editor_camera, { selected_entityID, active_scene.get() }, gizmo_type);
	ImGui::End();


	scene_hierarchy_panel.OnImGuiRender();
	component_panel.SetSelevted_EntityHandle(selected_entityID);
	component_panel.OnImGuiRender();

	DrawToolBar();

	ImGuiRenderer::Instance()->GuiDrawDockSpaceEnd();
}

void EditorLayer::OnEvent(Event& event)
{
	EngineLog::Info("Event : {}", event);

	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowResizeEvent>([&](WindowResizeEvent& e)
	{
		EngineLog::Info("Got {}!", e.ToString());
		return true;
	});
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

void EditorLayer::SelectGuizmo()
{
	if(Input::IsPressed(KeyCode::Q))
		gizmo_type = -1;
	else if(Input::IsPressed(KeyCode::W))
		gizmo_type = (int)ImGuizmo::OPERATION::TRANSLATE;
	else if (Input::IsPressed(KeyCode::E))
		gizmo_type = (int)ImGuizmo::OPERATION::ROTATE;
	else if (Input::IsPressed(KeyCode::R))
		gizmo_type = (int)ImGuizmo::OPERATION::SCALE;
}

void EditorLayer::DrawGuizmo(EditorCamera& camera, Entity entity, int GizmoType)
{
	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();

	float windowWidth = (float)ImGui::GetWindowWidth();
	float windowHeight = (float)ImGui::GetWindowHeight();
	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
	//editor camera
	const glm::mat4& cameraProjection = camera.GetProjection();
	glm::mat4 cameraView = camera.GetViewMatrix();

	if(!entity || GizmoType == -1)
	{
		return;
	}

	auto& tc = entity.GetComponent<TransformComponent>();
	glm::mat4 transform = tc.GetTransform();

	//snapping
	bool snap = Input::IsPressed(Key::LeftControl);
	float snapValue = 0.5f;// snap 0.5 for translation and scale
	if (GizmoType == ImGuizmo::OPERATION::ROTATE)
		snapValue = 45.0f;

	float snapValues[3] = { snapValue,snapValue,snapValue };

	ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
		(ImGuizmo::OPERATION)GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

	if (ImGuizmo::IsUsing())
	{
		glm::vec3 translation, rotation, scale;
		ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale));
		tc.Position = translation;
		tc.Rotation = glm::radians(rotation);
		tc.Scale = scale;
	}
}

void EditorLayer::OnScenePlay()
{
	scene_state = SceneState::Play;
	active_scene->StartRuntime();
}

void EditorLayer::OnSceneStop()
{
	scene_state = SceneState::Edit;
	active_scene->StopRuntime();
}

void EditorLayer::SetNewScene(std::shared_ptr<Scene> new_scene) {
	selected_entityID = entt::null;
	active_scene = new_scene;
	component_panel.SetScene(active_scene);
	scene_hierarchy_panel.SetScene(active_scene);
	active_scene->Awake();
	active_scene->Start();
}
