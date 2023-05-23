#pragma once
#include <memory>

class Scene;

class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(std::shared_ptr<Scene> scene);

	void SetScene(std::shared_ptr<Scene> scene);

	void OnImGuiRender();
private:
	std::shared_ptr<Scene> m_scene;
};