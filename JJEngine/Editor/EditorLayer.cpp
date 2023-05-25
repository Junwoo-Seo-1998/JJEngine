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

	component_panel.SetScene(Application::Instance().GetSceneManager()->GetCurrentScene());
	scene_hierarchy_panel.SetScene(Application::Instance().GetSceneManager()->GetCurrentScene());
	scene_hierarchy_panel.SetSlected_EntityFunc([&](entt::entity ID)->void {selected_entityID = ID; });

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
	//get something from scene
}

void EditorLayer::OnPreRender()
{
	editor_viewport->Bind(true);
	glViewport(0, 0, 400, 400);
}

void EditorLayer::OnRender()
{
	glm::mat4 identity(1.0f);
	auto& reg = active_scene->GetRegistry();
	auto viewProj = identity;
	
	viewProj *= glm::perspective(400.f, 1.f, 0.001f, 100.f);
	viewProj *=MatrixMath::BuildCameraMatrixWithDirection({ 0,0,1 }, { 0,0,-1.f });

	Renderer2D::BeginScene(viewProj);

	auto group=reg.group<TransformComponent, SpriteRendererComponent>();

	for (auto entity:group)
	{
		auto [trans, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
		trans.Scale = {1.2,1.1,1 };
		Renderer2D::DrawQuad(trans.GetTransform(), { 1,1,1,1});
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
	ImGui::Image((void*)editor_viewport->GetColorTexture(0)->GetTextureID(), { 400, 400 }, { 0,1 }, { 1,0 });
	ImGui::End();


	scene_hierarchy_panel.OnImGuiRender();
	component_panel.SetSelevted_EntityHandle(selected_entityID);
	component_panel.OnImGuiRender();

	ImGuiRenderer::Instance()->GuiDrawDockSpaceEnd();
}
