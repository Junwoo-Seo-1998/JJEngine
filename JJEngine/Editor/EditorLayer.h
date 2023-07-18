#pragma once
#include "Core/Layer/Layer.h"
#include "Panel/SceneHierarchyPanel.h"
#include "Panel/AssetBrowserPanel.h"
#include "Panel/ProjectSettingPanel.h"
#include "Panel/ComponentPanel.h"
#include "Core/Graphics/Renderer/EditorCamera.h"
#include <entt/entt.hpp>
#include <memory>
#include <glm/glm.hpp>

#include "Core/ImGui/ImGuiSubWindow.h"

#include "Panel/PanelMessenger.h"
class Scene;
class FrameBuffer;
class Asset_Scene;
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
	void SetNewScene(std::shared_ptr<Asset_Scene> new_scene);

private:
	enum class SceneState
	{
		Play = 0, Edit = 1
	};

	SceneState m_SceneState = SceneState::Edit;

	EditorCamera m_EditorCamera;
	glm::vec2 m_ViewportSize{ 400.f, 400.f };
	glm::vec2 m_ViewportBoundMin{};
	glm::vec2 m_ViewportBoundMax{};

	int m_GizmoType = -1;//no drawing

	//update stuff should be based on active scene
	std::shared_ptr<Scene> m_ActiveScene;
	//for play and pause
	std::shared_ptr<Scene> m_EditorScene, m_RuntimeScene;

	//for drawing
	std::shared_ptr<SceneRenderer> m_SceneRenderer;
	std::shared_ptr<FrameBuffer> m_EditorSelectionViewport;

	entt::entity m_SelectedEntityID{entt::null};
	std::filesystem::path shouldOpenFile{""};

	PanelMessenger messenger{};

	ComponentPanel m_ComponentPanel{messenger};
	SceneHierarchyPanel m_SceneHierarchyPanel{messenger};
	AssetBrowserPanel m_AssetBrowserPanel{messenger};
	ProjectSettingPanel m_ProjectSettingPanel{messenger};

	ImGuiSubWindow m_AssetBrowserWindow{ "Asset browser" };

	//std::shared_ptr<FrameBuffer> m_EditorViewport;

	std::shared_ptr<Texture> m_PlayIcon;
	std::shared_ptr<Texture> m_StopIcon;
};
