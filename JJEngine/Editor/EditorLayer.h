#pragma once
#include "Core/Layer/Layer.h"

class EditorLayer : public Layer
{
public:
	virtual ~EditorLayer() override;
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
};
