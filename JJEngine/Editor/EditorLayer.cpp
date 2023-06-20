#include "Core/SceneSerializer.h"
#include "EditorLayer.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "TestScene.h"
#include "Test3DScene.h"
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
#include "Core/Graphics/RenderCommand.h"
#include "Core/Input/Input.h"

#include "Core/Graphics/Graphics.h"
#include "Core/Script/ScriptEngine.h"

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnAttach()
{
	Log::Info("Editor Layer Added");
	m_EditorCamera = EditorCamera{ 45.f, 1.f, 0.01f, 100.f };
	SetNewScene(std::make_shared<Test3DScene>());
	//for testing
	m_EditorViewport = FrameBuffer::CreateFrameBuffer({ 400,400,{FrameBufferFormat::RGBA, FrameBufferFormat::R_INT,FrameBufferFormat::Depth } });
	m_EditorSelectionViewport = FrameBuffer::CreateFrameBuffer({ 400, 400, {FrameBufferFormat::R_INT, FrameBufferFormat::Depth} });
	m_PlayIcon = Texture::CreateTexture(File::ReadImageToTexture("Resources/Textures/UI/PlayButton.png"));
	m_StopIcon = Texture::CreateTexture(File::ReadImageToTexture("Resources/Textures/UI/Stop.png"));

	Graphics::GetInstance()->SetFinalFBOID(m_EditorViewport->GetHandle());
}

void EditorLayer::OnDetach()
{

}

void EditorLayer::OnStart()
{
	m_ComponentPanel.SetScene(m_ActiveScene);
	m_SceneHierarchyPanel.SetScene(m_ActiveScene);

	m_AssetBrowserPanel.Set();
	//m_AssetBrowserWindow.Push_ImGuiCommand([&]()->void {m_AssetBrowserPanel.OnImGuiRender_ResorceHierarchy("."); });
	m_AssetBrowserWindow.Push_ImGuiCommand([&]()->void {m_AssetBrowserPanel.OnImGuiRender(); });
}

void EditorLayer::OnUpdate()
{
	if(Input::IsTriggered(KeyCode::P))
	{
		Script::ScriptEngine::ReloadAssembly();
	}

	switch (m_SceneState)
	{
	case SceneState::Edit:
		m_EditorCamera.OnUpdate();
		SelectGuizmo();
		break;
	case SceneState::Play:
		//place holder
		break;
	}


	
	if (shouldOpenFile.extension().string() == ".scn") {
		//TODO: ask save now scene
		SetNewScene(std::make_shared<Scene>(shouldOpenFile.filename().string()));
		SceneSerializer see_real(m_ActiveScene);
		m_ActiveScene->SetScenePath(shouldOpenFile);
		if (see_real.Deserialize(shouldOpenFile.string()) == false) Log::Error("Fail to deserialize Scene: " + shouldOpenFile.filename().string());
	}
	shouldOpenFile.clear();
	
	//Message handling
	if (messenger.HasMessage()){
		std::string message = messenger.ReadMessage();
		if (message == ENTITY_SELECTED) {
			messenger.DeleteMessage();
			m_SelectedEntityID = (entt::entity)std::stoul(messenger.ReadMessage());
		}
		else if (message == FILE_OPEN) {
			messenger.DeleteMessage();
			shouldOpenFile = messenger.ReadMessage();
		}
		else {
			//send message to panels
		}
		messenger.DeleteMessage();
	}

	if (Input::IsTriggered(MouseCode::Left) && m_SceneState != SceneState::Play && !ImGuizmo::IsOver() && !Input::IsPressed(KeyCode::LeftAlt))
	{
		auto [mx, my] = ImGui::GetMousePos();

		mx -= m_ViewportBoundMin.x;
		my -= m_ViewportBoundMin.y;
		glm::vec2 viewportSize = m_ViewportBoundMax - m_ViewportBoundMin;
		my = viewportSize.y - my;

		//EngineLog::Info("viewportSize {}, {}", viewportSize.x, viewportSize.y);
		if (mx >= 0 && mx <= viewportSize.x && my >= 0 && my <= viewportSize.y)
		{
			//EngineLog::Info("Mouse {}, {}", mx, my);
			m_EditorSelectionViewport->Bind();
			int SelectedEntityID = m_EditorSelectionViewport->GetPixelInt(0, mx, my);
			EngineLog::Info("Mouse {}, {} Selected EntityID: {}", mx, my, SelectedEntityID);
			if(SelectedEntityID!=-1)
			{
				m_SelectedEntityID = static_cast<entt::entity>(SelectedEntityID);
			}
			else
			{
				m_SelectedEntityID = entt::null;
			}
			m_EditorSelectionViewport->UnBind();
		}
	}
}

void EditorLayer::OnPreRender()
{
	auto spec = m_EditorViewport->GetSpecification();
	const bool viewportSizeChanged = (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y);
	if (viewportSizeChanged && m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f)
	{
		m_EditorCamera.SetViewportSize((unsigned)m_ViewportSize.x, (unsigned)m_ViewportSize.y);
		m_ActiveScene->ResizeViewport((unsigned)m_ViewportSize.x, (unsigned)m_ViewportSize.y);
		m_EditorViewport->Resize((unsigned)m_ViewportSize.x, (unsigned)m_ViewportSize.y);
		m_EditorSelectionViewport->Resize((unsigned)m_ViewportSize.x, (unsigned)m_ViewportSize.y);
	}
	m_EditorViewport->Bind();
	RenderCommand::Clear();
}

void EditorLayer::OnRender()
{
	if (!m_ActiveScene)
		return;
	switch (m_SceneState)
	{
	case SceneState::Edit:
		m_ActiveScene->UpdateEditor(m_EditorCamera);

		break;
	case SceneState::Play:
		m_ActiveScene->UpdateRuntime();
		break;
	}
}

void EditorLayer::OnPostRender()
{
	m_EditorViewport->UnBind();
	if (m_SceneState != SceneState::Play)
	{
		m_EditorSelectionViewport->Bind();
		RenderCommand::Clear();
		m_EditorSelectionViewport->GetColorTexture(0)->ClearTexture(-1);
		m_EditorScene->RenderEntityID(m_EditorCamera);
		m_EditorSelectionViewport->UnBind();
	}
}

void EditorLayer::OnImGuiRender()
{
	static bool openMakeSceneWindow{false};
	ImGuiRenderer::Instance()->GuiDrawDockSpaceBegin();
	if (ImGui::BeginMenuBar()) // testing for now
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Make scene")) {
				openMakeSceneWindow = true;
			}
			if (ImGui::MenuItem("Save scene")) {
				SceneSerializer see_real(m_ActiveScene);
				if (m_ActiveScene->GetScenePath().extension().string() == ".scn") {
					see_real.Serialize(m_ActiveScene->GetScenePath().string());
				}
				else {
					see_real.Serialize("./Resources/Scenes/" + m_ActiveScene->GetSceneName() + ".scn");
				}
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	if (openMakeSceneWindow == true) { // Scene generator window
		ImGui::Begin("Scene generator");
		static char newSceneName[128] = "Default";
		ImGui::InputText("Input scene name", newSceneName, IM_ARRAYSIZE(newSceneName));
		//ImGui::SameLine();
		if (ImGui::Button("Make") == true) {
			openMakeSceneWindow = false;
			SetNewScene(std::make_shared<Scene>(newSceneName));
			SceneSerializer see_real(m_ActiveScene);
			see_real.Serialize("./Resources/Scenes/" + m_ActiveScene->GetSceneName() + ".scn");
		}
		ImGui::End();
	}

	m_AssetBrowserWindow.Update();


	//viewport part
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
		ImGui::Begin("viewport");

		auto viewportOffset = ImGui::GetCursorPos();//include tab
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();

		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;
		vMax.x += ImGui::GetWindowPos().x;
		vMax.y += ImGui::GetWindowPos().y;

		m_ViewportBoundMax = { vMax.x, vMax.y };
		m_ViewportBoundMin = { vMin.x, vMin.y };

		ImGui::Image((void*)m_EditorViewport->GetColorTexture(0)->GetTextureID(), viewportPanelSize, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		if (m_SceneState == SceneState::Edit)
			DrawGuizmo(m_EditorCamera, { m_SelectedEntityID, m_ActiveScene.get() }, m_GizmoType);
		ImGui::End();
		ImGui::PopStyleVar();
	}

	m_SceneHierarchyPanel.OnImGuiRender();
	m_ComponentPanel.SetSelevted_EntityHandle(m_SelectedEntityID);
	m_ComponentPanel.OnImGuiRender();

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

	std::shared_ptr<Texture> TextureToUse = m_SceneState == SceneState::Edit ? m_PlayIcon : m_StopIcon;
	float buttonSize = ImGui::GetWindowHeight() - 4.0f;
	ImGui::SameLine((ImGui::GetContentRegionMax().x * 0.5f) - (buttonSize * 0.5f));
	if (ImGui::ImageButton((ImTextureID)TextureToUse->GetTextureID(),
		ImVec2{ buttonSize, buttonSize }, ImVec2{ 0.f, 0.f }, ImVec2{ 1.f, 1.f }, 0))
	{
		if (m_SceneState == SceneState::Edit)
		{
			OnScenePlay();
		}
		else if (m_SceneState == SceneState::Play)
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
		m_GizmoType = -1;
	else if(Input::IsPressed(KeyCode::W))
		m_GizmoType = (int)ImGuizmo::OPERATION::TRANSLATE;
	else if (Input::IsPressed(KeyCode::E))
		m_GizmoType = (int)ImGuizmo::OPERATION::ROTATE;
	else if (Input::IsPressed(KeyCode::R))
		m_GizmoType = (int)ImGuizmo::OPERATION::SCALE;
}

void EditorLayer::DrawGuizmo(EditorCamera& camera, Entity entity, int GizmoType)
{
	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();

	ImGuizmo::SetRect(m_ViewportBoundMin.x, m_ViewportBoundMin.y, m_ViewportBoundMax.x - m_ViewportBoundMin.x, m_ViewportBoundMax.y - m_ViewportBoundMin.y);

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
	m_SceneState = SceneState::Play;

	m_RuntimeScene = Scene::Copy(m_EditorScene);
	m_RuntimeScene->ResizeViewport((unsigned)m_ViewportSize.x, (unsigned)m_ViewportSize.y);
	m_RuntimeScene->Awake();
	m_RuntimeScene->Start();
	m_RuntimeScene->StartRuntime();
	m_ActiveScene = m_RuntimeScene;
}

void EditorLayer::OnSceneStop()
{
	m_SceneState = SceneState::Edit;

	m_RuntimeScene->StopRuntime();
	m_RuntimeScene = nullptr;

	m_ActiveScene = m_EditorScene;
}

void EditorLayer::SetNewScene(std::shared_ptr<Scene> new_scene) {
	m_SelectedEntityID = entt::null;
	if (m_SceneState != SceneState::Edit)
		OnSceneStop();

	m_EditorScene = new_scene;
	m_EditorScene->ResizeViewport((unsigned)m_ViewportSize.x, (unsigned)m_ViewportSize.y);
	m_SceneHierarchyPanel.SetScene(m_EditorScene);
	m_ComponentPanel.SetScene(m_EditorScene);

	m_ActiveScene = m_EditorScene;
	m_ActiveScene->Awake();
	m_ActiveScene->Start();
}
