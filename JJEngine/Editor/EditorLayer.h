#pragma once
#include "Core/Layer/Layer.h"
#include "Panel/SceneHierarchyPanel.h"

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



	std::shared_ptr<FrameBuffer> editor_viewport;
};
