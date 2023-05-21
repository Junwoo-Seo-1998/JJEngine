#pragma once
#include "Core/Scene.h"
#include <string>
#include <iostream>

#include "glad.h"
#include "Core/Graphics/Shader.h"
#include "Core/Graphics/VertexArray.h"
#include "Core/Graphics/VertexBuffer.h"

// for testing component manager
#include "Core/ComponentManager.h"
#include "Core/Graphics/Texture.h"
#include "Core/Utils/File.h"
#include "imgui.h"
#include "Core/Graphics/FrameBuffer.h"
#include "Core/Graphics/IndexBuffer.h"

#include <Core/Utils/Log.h>
#include <Core/entt.h>
#include <Core/EventManager.h>
#include <Core/EventListener.h>

#include "Core/Event/StringEvent.h"


class TestScene : public Scene
{
	Registry registry{};
	ID eventListener{};
	EventManager* evManager{};

	std::string text{};
	std::shared_ptr<VertexArray> vertex_array;
	std::shared_ptr<VertexBuffer> vertex_buffer;
	std::shared_ptr<IndexBuffer> index_buffer;
	std::shared_ptr<VertexBuffer> vertex_color_buffer;
	std::shared_ptr<FrameBuffer> framebuffer;
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Texture> test_texture;
public:
	TestScene(std::string);
	~TestScene();
	void Load() override
	{
		std::cout << text << std::endl;
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

		framebuffer = FrameBuffer::CreateFrameBuffer({ 1200, 800, { FrameBufferFormat::RGB } });
		glClearColor(0.3, 0.7, 0.3, 1.0);


		eventListener = registry.create();
		registry.emplace<EventListener>(eventListener);
		evManager = new EventManager{&registry};
	};
	void Update(double dt) override
	{

		Scene::Update(dt);
		framebuffer->Bind();
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
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		ImGuiRenderer::Instance()->GuiDrawDockSpaceBegin();

		ImGui::Begin("Button test");
		if (ImGui::Button("Button 1")) {
			evManager->notifyEvent(1);
			Log::Info("event notified");
		}
		if (ImGui::Button("Button 2")) {
			evManager->notifyEvent(2);
			Log::Info("event notified");
		}
		if (ImGui::Button("Button 3")) {
			evManager->notifyEvent(3);
			Log::Info("event notified");
		}
		ImGui::End();

		/*evManager->Update();
		EventListener& listener = registry.get<EventListener>(eventListener);
		int ev = listener.GetNextEvent();
		if (ev == 1) {
			Log::Info("Button_1 clicked");
		}
		if (ev == 2) {
			Log::Info("Button_2 clicked");
		}
		if (ev == 3) {
			Log::Info("Button_3 clicked");
		}*/

		//for imgui test and framebuffer
		/*ImGui::Begin("framebuffer test");
		constexpr ImVec2 size{ 480,320 };
		unsigned textureID = framebuffer->GetColorTexture(0)->GetTextureID();
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		ImGui::End();*/

		ImGuiRenderer::Instance()->GuiDrawDockSpaceEnd();
	}
	void Draw() override {}
	void Unload()  override {}
private:

};

TestScene::TestScene(std::string t) :Scene(), text(t)
{

}

TestScene::~TestScene()
{
	delete evManager;
}