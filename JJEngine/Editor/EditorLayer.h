#pragma once
#include "Core/Layer/Layer.h"
#include "Panel/SceneHierarchyPanel.h"
#include "Panel/AssetBrowserPanel.h"
#include "Panel/ComponentPanel.h"
#include <entt/entt.hpp>
#include <memory>
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
private:
	void DrawToolBar();
	void OnScenePlay();
	void OnSceneStop();

private:
	enum class SceneState
	{
		Play = 0, Edit = 1
	};

	SceneState scene_state = SceneState::Edit;


	std::shared_ptr<Scene> active_scene;
	entt::entity selected_entityID{entt::null};

	ComponentPanel component_panel;
	SceneHierarchyPanel scene_hierarchy_panel;
	AssetBrowserPanel ABP{};

	entt::registry editorRegistry{};
	std::shared_ptr<FrameBuffer> editor_viewport;

	std::shared_ptr<Texture> PlayIcon;
	std::shared_ptr<Texture> StopIcon;
};
