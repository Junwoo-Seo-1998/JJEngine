#pragma once
#include <Core/Layer/Layer.h>
#include <memory>

class Asset_Scene;
class SceneRenderer;
class RuntimeLayer : public Layer {
	std::shared_ptr<Asset_Scene> m_ActiveScene;
	std::shared_ptr<SceneRenderer> m_SceneRenderer;


public:
	~RuntimeLayer()override;
	void OnAttach() override;
	void OnDetach() override;
	void OnStart() override;
	void OnUpdate() override;
	void OnPreRender() override;
	void OnRender() override;
	void OnPostRender() override;
	void OnImGuiRender() override;
	void OnEvent(Event& event) override;
};