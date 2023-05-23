#include "EditorLayer.h"
#include "glad.h"
#include "imgui.h"
#include "Core/ImGui/ImGuiRenderer.h"
#include "Core/Graphics/FrameBuffer.h"
#include "Core/Utils/Log.h"
#include "Core/Application.h"
#include "Core/SceneManager.h"

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnAttach()
{
	Log::Info("Editor Layer Added");
	//for testing
	editor_viewport = FrameBuffer::CreateFrameBuffer({ 400,400,{FrameBufferFormat::RGB} });
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
	editor_viewport->Bind(true);
}

void EditorLayer::OnRender()
{
}

void EditorLayer::OnPostRender()
{
	editor_viewport->UnBind();
}

void EditorLayer::OnImGuiRender()
{
	ImGuiRenderer::Instance()->GuiDrawDockSpaceBegin();
	ImGui::Begin("viewport");
	ImGui::Image((void*)editor_viewport->GetColorTexture(0)->GetTextureID(), { 400, 400 }, { 0,1 }, { 1,0 });
	ImGui::End();


	scene_hierarchy_panel.OnImGuiRender();
	ImGuiRenderer::Instance()->GuiDrawDockSpaceEnd();
}
