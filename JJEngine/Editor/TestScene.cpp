#include "TestScene.h"
#include "Core/Graphics/Texture.h"
#include "Core/Utils/File.h"
#include "imgui.h"
#include <Core/Utils/Log.h>
#include "Core/Time.h"
#include "Core/Component/CameraComponent.h"
#include "Core/Entity/Entity.hpp"
#include "Core/Component/SpriteRendererComponent.h"
#include "Core/Component/RigidBody2DComponent.h"
#include "Core/Component/BoxCollider2DComponent.h"
TestScene::TestScene(std::string t)
	:Scene(t)
{

}

void TestScene::Start()
{
	Log::Info("Scene: " + m_scene_name);
	shader = Shader::CreateShaderFromFile({
{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/simple.vert"}},
{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/simple.frag"} }
		});
	shader->Use();
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};
	float colors[] = {
		// colors
		1.0f, 0.0f, 0.0f,   // bottom right
		0.0f, 1.0f, 0.0f,   // bottom left
		0.0f, 0.0f, 1.0f    // top 
	};
	float verticesWithColor[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

	float textureQuad[] = {
		// positions        // texture coords
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // top right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // bottom left
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,    // top left 
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // bottom left
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // top right
	};

	float textureQuadIndexed[] = {
		// positions        // texture coords
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // bottom left
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right
		0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // top right
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,    // top left 
	};

	vertex_array = VertexArray::CreateVertexArray();
	vertex_array->Bind();
	vertex_buffer = VertexBuffer::CreateVertexBuffer(sizeof(textureQuadIndexed));
	vertex_buffer->SetData(sizeof(textureQuadIndexed), textureQuadIndexed);
	vertex_buffer->SetDataTypes({
		{0, DataType::Float3},//location=0, pos
		{1, DataType::Float2},//location=1, uv
		});
	vertex_buffer->BindToVertexArray();


	unsigned int textureQuadIndices[] = {
		0,1,2,
		0,2,3,
	};

	index_buffer = IndexBuffer::CreateIndexBuffer(sizeof(textureQuadIndices));
	index_buffer->SetData(sizeof(textureQuadIndices), textureQuadIndices);
	index_buffer->BindToVertexArray();
	//vertex_color_buffer = VertexBuffer::CreateVertexBuffer(sizeof(colors));
	//vertex_color_buffer->SetData(sizeof(colors), colors);
	//vertex_color_buffer->SetDataTypes({ {1,DataType::Float3} });
	//vertex_color_buffer->BindToVertexArray();


	test_texture = Texture::CreateTexture(File::ReadImageToTexture("Resources/Textures/test.jpg"));

	framebuffer = FrameBuffer::CreateFrameBuffer({ 1200, 800, { FrameBufferFormat::RGBA } });

	auto cam=CreateEntity("Camera");
	auto& camComp=cam.AddComponent<CameraComponent>();
	camComp.IsMainCamera = true;
	cam.Transform().Position = { 0,1.52f,7.5f };

	auto ground = CreateEntity("Ground");
	auto& groundSprite = ground.AddComponent<SpriteRendererComponent>();
	groundSprite.color = { 0.5f,0.25f, 0.2f, 1.f };
	ground.Transform().Scale = { 5,0.25f,1 };
	ground.Transform().Position.y = -1.f;
	ground.AddComponent<BoxCollider2DComponent>();
	ground.AddComponent<RigidBody2DComponent>();

	auto ground2 = CreateEntity("Ground2");
	auto& groundSprite2 = ground2.AddComponent<SpriteRendererComponent>();
	groundSprite2.color = { 0.5f,0.25f, 0.2f, 1.f };
	ground2.Transform().Scale = { 1.f,0.25f,1 };
	ground2.Transform().Position.y = 1.21f;
	ground2.Transform().Position.x = 0.67f;
	ground2.Transform().Rotation.z = glm::radians(29.23f);
	ground2.AddComponent<BoxCollider2DComponent>();
	ground2.AddComponent<RigidBody2DComponent>();

	auto temp = CreateEntity("Test Texture Entity");

	temp.Transform().Position = { 0.f, 2.50f, 0 };
	auto& sprite = temp.AddComponent<SpriteRendererComponent>();
	sprite.texture = Texture::CreateTexture(File::ReadImageToTexture("Resources/Textures/test.jpg"));

	temp.AddComponent<BoxCollider2DComponent>();
	auto& body = temp.AddComponent<RigidBody2DComponent>();
	body.Type = RigidBody2DComponent::BodyType::Dynamic;
}

void TestScene::Update()
{
	float dt = Time::GetDelta();
	/*framebuffer->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//draw test
	shader->Use();
	vertex_array->Bind();
	shader->SetInt("testTexture", 0);
	test_texture->BindTexture();
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	framebuffer->UnBind();
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);*/
}
