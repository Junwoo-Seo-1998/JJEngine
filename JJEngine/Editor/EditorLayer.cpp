#include "EditorLayer.h"
#include "imgui.h"
#include "Core/ImGui/ImGuiRenderer.h"
#include "Core/Utils/Log.h"
#include "Core/Application.h"
#include "Core/SceneManager.h"
#include "Core/ImGui/ImGuiSubWindow.h"
#include "Panel/AssetBrowserPanel.h"
EditorLayer::~EditorLayer()
{
	editorRegistry.clear();
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
	entt::entity ID{ editorRegistry.create() };
	ImGuiSubWindow* temp = &editorRegistry.emplace<ImGuiSubWindow>(ID, "Asset browser");
	temp->Push_ImGuiCommand([&]()->void {ABP.OnImGuiRender(); });

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
	auto panels = editorRegistry.view<ImGuiSubWindow>();
	for (auto& ID:panels) {
		panels.get<ImGuiSubWindow>(ID).Update();
	}
	scene_hierarchy_panel.OnImGuiRender();
	ImGuiRenderer::Instance()->GuiDrawDockSpaceEnd();
}
