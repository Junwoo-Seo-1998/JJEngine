#include "EditorLayer.h"
#include "imgui.h"
#include "Core/ImGui/ImGuiRenderer.h"
#include "Core/Utils/Log.h"
#include "Core/Application.h"
#include "Core/SceneManager.h"

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnAttach()
{
	Log::Info("Editor Layer Added");
}

void EditorLayer::OnDetach()
{

}

void EditorLayer::OnStart()
{
	scene_hierarchy_panel.SetScene(Application::Instance().GetSceneManager()->GetCurrentScene());
}

void EditorLayer::OnUpdate()
{
	//get something from scene
}

void EditorLayer::OnPreRender()
{

}

void EditorLayer::OnRender()
{

}

void EditorLayer::OnPostRender()
{

}

void EditorLayer::OnImGuiRender()
{
	ImGuiRenderer::Instance()->GuiDrawDockSpaceBegin();

	scene_hierarchy_panel.OnImGuiRender();
	ImGuiRenderer::Instance()->GuiDrawDockSpaceEnd();
}
