#include "glad.h"
#include "Core/Utils/Math/MatrixMath.h"

#include "IngameRendererLayer.h"
#include "Core/Utils/Log.h"
#include "Core/Application.h"
#include "Core/Entity/Entity.hpp"

#include "Core/Scene.h"
#include "Core/SceneManager.h"

#include "Core/Graphics/VertexArray.h"
#include "Core/Graphics/FrameBuffer.h"
#include "Core/Graphics/Mesh.h"
#include "Core/Graphics/Renderer/Renderer.h"


#include "Core/Component/LightComponent.h"
#include "Core/Component/CameraComponent.h"
#include "Core/Component/TransformComponent.h"

#include "Core/Window.h"
#include "ShadowScene.h"

IngameRendererLayer::~IngameRendererLayer()
{
}

void IngameRendererLayer::OnAttach()
{
	active_scene = sc;
	const glm::uvec2 shadowResolution{ 512, 512 };
	auto window = Application::Instance().GetWindow();
	Log::Info("Ingame Renderer Layer Added");
	//for testing
	renderer_vao = VertexArray::CreateVertexArray();
	//Position, Normal, Ambient, Diffuse, Specular 
	g_buffer = FrameBuffer::CreateFrameBuffer({ static_cast<unsigned int>(get<0>(window->GetWidthAndHeight())), static_cast<unsigned int>(get<1>(window->GetWidthAndHeight())),{FrameBufferFormat::RGBA32F, FrameBufferFormat::RGBA32F, FrameBufferFormat::RGBA, FrameBufferFormat::RGBA, FrameBufferFormat::RGBA}});

	shadow_buffer = FrameBuffer::CreateFrameBuffer({ shadowResolution.x,shadowResolution.y,{FrameBufferFormat::Depth } });
	
}

void IngameRendererLayer::OnDetach()
{

}

void IngameRendererLayer::OnStart()
{

	auto& reg = active_scene->GetRegistry();

	auto view = reg.view<Model>();

	for (auto entity : view)
	{
		Entity e( entity, active_scene.get() );
		auto& m = e.GetComponent<Model>();
		for (auto& mesh : m)
		{
			mesh->GetMeshVBO() = VertexBuffer::CreateVertexBuffer(mesh->GetNumOfVertices() * sizeof(Vertex));
			mesh->GetMeshEBO() = IndexBuffer::CreateIndexBuffer(mesh->GetNumOfIndices() * sizeof(int));
		}
	}
}

void IngameRendererLayer::OnUpdate()
{

	//get something from scene
}

void IngameRendererLayer::OnPreRender()
{

}

void IngameRendererLayer::OnRender()
{
	auto& reg = active_scene->GetRegistry();

	auto cameraView = reg.view<CameraComponent>();
	auto objectView = reg.view<Model>();
	auto lightView = reg.view<LightComponent>();
	int a = cameraView.size();
	for (auto& cam : cameraView)
	{
		Entity camEntity(cam, active_scene.get());
		auto& camera = camEntity.GetComponent<CameraComponent>();
		auto& cameraTransform = camEntity.GetComponent<TransformComponent>();

		glm::mat4 camVP = camera.GetProjection() * MatrixMath::BuildCameraMatrix(cameraTransform.Position, cameraTransform.Position + cameraTransform.GetForward(), cameraTransform.GetUp());
		for (auto& obj : objectView)
		{
			Entity objEntity(obj, active_scene.get());
			auto& transform = objEntity.GetComponent<TransformComponent>();
			auto& model = objEntity.GetComponent<Model>();
			auto& material = objEntity.GetComponent<MaterialComponent>();

			Renderer::BeginScene(camVP);
			Renderer::SetModel(model);
			Renderer::SetMaterial(material);
			Renderer::SetVAO(renderer_vao);
			Renderer::SetShadowBuffer(shadow_buffer);
			Renderer::SetShadowInformation(glm::ivec2{ 512, 512 }, glm::ivec2{ 1, 1 });
			Renderer::SetTransform(transform);

			for (auto& light : lightView)
			{
				Entity lightEntity(light, active_scene.get());
				auto& light = lightEntity.GetComponent<LightComponent>();
				auto& lightTransform = lightEntity.GetComponent<TransformComponent>();
				Renderer::AddAffectLight(light, lightTransform);
			}
			Renderer::EndScene();
		}
	}

}

void IngameRendererLayer::OnPostRender()
{
	Renderer::DrawAllScene();
}

void IngameRendererLayer::OnImGuiRender()
{

}
