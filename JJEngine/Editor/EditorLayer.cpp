#include "EditorLayer.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "glad.h"
#include "imgui.h"
#include "Core/ImGui/ImGuiRenderer.h"
#include "Core/Graphics/FrameBuffer.h"
#include "Core/Utils/Log.h"
#include "Core/Application.h"
#include "Core/Scene.h"
#include "Core/SceneManager.h"
#include "Core/ImGui/ImGuiSubWindow.h"
#include "Panel/AssetBrowserPanel.h"
#include "Core/Component/SpriteRendererComponent.h"
#include "Core/Component/TransformComponent.h"
#include "Core/Entity/Entity.hpp"
#include "Core/Graphics/Renderer/Renderer2D.h"

#include "Core/Utils/File.h"
#include "Core/Utils/Math/MatrixMath.h"

EditorLayer::~EditorLayer()
{
	editorRegistry.clear();
}

void EditorLayer::OnAttach()
{
	Log::Info("Editor Layer Added");
	active_scene = std::make_shared<Scene>();
	//for testing
	editor_viewport = FrameBuffer::CreateFrameBuffer({ 400,400,{FrameBufferFormat::RGBA, FrameBufferFormat::Depth } });
}

void EditorLayer::OnDetach()
{

}

void EditorLayer::OnStart()
{


	scene_hierarchy_panel.SetScene(Application::Instance().GetSceneManager()->GetCurrentScene());
	entt::entity ID{ editorRegistry.create() };
	ImGuiSubWindow* temp = &editorRegistry.emplace<ImGuiSubWindow>(ID, "Asset browser");
	ABP.Set();
	temp->Push_ImGuiCommand([&]()->void {ABP.OnImGuiRender(); });



	auto test_texture = Texture::CreateTexture(File::ReadImageToTexture("Resources/Textures/test.jpg"));
	Entity entity=active_scene->CreateEntity();
	entity.AddComponent<SpriteRendererComponent>(test_texture);
}

void EditorLayer::OnUpdate()
{
	active_scene = Application::Instance().GetSceneManager()->GetCurrentScene();
	//get something from scene
}

void EditorLayer::OnPreRender()
{
	editor_viewport->Bind(true);
	glViewport(0, 0, 400, 400);
}

void EditorLayer::OnRender()
{
	if(!active_scene)
		return;

	auto& reg = active_scene->GetRegistry();
	glm::mat4 viewProj = glm::perspective(400.f, 1.f, 0.001f, 100.f);
	viewProj *=MatrixMath::BuildCameraMatrixWithDirection({ 1,0,1.f }, { 0,0,-1.f });
	

	Renderer2D::BeginScene(viewProj);

	auto group=reg.group<TransformComponent, SpriteRendererComponent>();

	for (auto entity:group)
	{
		Entity entity{ e, active_scene.get() };
		//Log::Info("To Draw: {}", entity.Name());
		Renderer2D::DrawQuad(entity.GetWorldSpaceTransformMatrix(), { 1,1,1,1 });
	}
	Renderer2D::EndScene();
}

void EditorLayer::OnPostRender()
{
	editor_viewport->UnBind();
}

void EditorLayer::OnImGuiRender()
{
	ImGuiRenderer::Instance()->GuiDrawDockSpaceBegin();
	auto panels = editorRegistry.view<ImGuiSubWindow>();
	for (auto& ID:panels) {
		panels.get<ImGuiSubWindow>(ID).Update();
	}

	ImGui::Begin("viewport");
	ImGui::Image((void*)editor_viewport->GetColorTexture(0)->GetTextureID(), { 400, 400 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	ImGui::End();


	scene_hierarchy_panel.OnImGuiRender();
	ImGuiRenderer::Instance()->GuiDrawDockSpaceEnd();
}
