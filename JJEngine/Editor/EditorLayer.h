#pragma once
#include "Core/Layer/Layer.h"
#include "Panel/SceneHierarchyPanel.h"
#include "Panel/AssetBrowserPanel.h"
#include "Panel/ComponentPanel.h"
#include "Core/Graphics/Renderer/EditorCamera.h"
#include <entt/entt.hpp>
#include <memory>
#include <glm/glm.hpp>

#include "Core/ImGui/ImGuiSubWindow.h"
class Scene;
class FrameBuffer;
class EditorLayer : public Layer
{
public:
	virtual ~EditorLayer() override;
	void OnAttach() override;
	void OnDetach() override;
	void OnStart() override;
	void OnUpdate() override;
	void OnPreRender() override;
	void OnRender() override;
	void OnPostRender() override;
	void OnImGuiRender() override;
	void OnEvent(Event& event) override;
private:
	void DrawToolBar();
	void SelectGuizmo();
	void DrawGuizmo(EditorCamera& camera, Entity entity, int GizmoType);
	void OnScenePlay();
	void OnSceneStop();
	void SetNewScene(std::shared_ptr<Scene> new_scene);

private:
	enum class SceneState
	{
		Play = 0, Edit = 1
	};

	SceneState m_SceneState = SceneState::Edit;

	EditorCamera m_EditorCamera;
	glm::vec2 m_ViewportSize{ 400.f, 400.f };

	int m_GizmoType = -1;//no drawing

	//update stuff should be based on active scene
	std::shared_ptr<Scene> m_ActiveScene;
	//for play and pause
	std::shared_ptr<Scene> m_EditorScene, m_RuntimeScene;
	

	entt::entity m_SelectedEntityID{entt::null};
	std::filesystem::path shouldOpenFile{""};

	ComponentPanel m_ComponentPanel;
	SceneHierarchyPanel m_SceneHierarchyPanel;
	AssetBrowserPanel m_AssetBrowserPanel{};

	ImGuiSubWindow m_AssetBrowserWindow{ "Asset browser" };

	std::shared_ptr<FrameBuffer> m_EditorViewport;

	std::shared_ptr<Texture> m_PlayIcon;
	std::shared_ptr<Texture> m_StopIcon;
};
