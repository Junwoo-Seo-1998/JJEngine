#pragma once
#include "Core/Layer/Layer.h"
#include "Panel/SceneHierarchyPanel.h"
#include "Panel/AssetBrowserPanel.h"
#include <entt/entt.hpp>

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
	SceneHierarchyPanel scene_hierarchy_panel;
	AssetBrowserPanel ABP{};
	entt::registry editorRegistry{};
	std::shared_ptr<FrameBuffer> editor_viewport;
};
