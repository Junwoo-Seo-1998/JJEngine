#pragma once
#include "Core/Scene.h"
#include <string>
#include <iostream>

#include "glad.h"
#include "Core/Graphics/Shader.h"
#include "Core/Graphics/VertexArray.h"
#include "Core/Graphics/VertexBuffer.h"


// for testing component manager
#include "Core/Entity.h"
#include "Core/ComponentManager.h"
#include "Core/Graphics/Texture.h"
#include "Core/Utils/File.h"
#include "imgui.h"
class A: public Entity {
	int a{ 1 };
public:
	void Update(double )override { std::cout << "A" << std::endl; }
};
class B : public Entity {
	short b{2};
public:
	void Update(double) override { std::cout << "B" << std::endl; }
};

class TestScene: public Scene
{
	std::string text{};
	std::shared_ptr<VertexArray> vertex_array;
	std::shared_ptr<VertexBuffer> vertex_buffer;
	std::shared_ptr<VertexBuffer> vertex_color_buffer;
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

		vertex_array = VertexArray::CreateVertexArray();
		vertex_array->Bind();
		vertex_buffer = VertexBuffer::CreateVertexBuffer(sizeof(vertices));
		vertex_buffer->SetData(sizeof(vertices), vertices);
		vertex_buffer->SetDataTypes({ {0, DataType::Float3} });
		vertex_buffer->BindToVertexArray();

		vertex_color_buffer = VertexBuffer::CreateVertexBuffer(sizeof(colors));
		vertex_color_buffer->SetData(sizeof(colors), colors);
		vertex_color_buffer->SetDataTypes({ {1,DataType::Float3} });
		vertex_color_buffer->BindToVertexArray();
		

		test_texture = Texture::CreateTexture(File::ReadImageToTexture("Resources/Textures/test.jpg"));
	};
	void Update(double dt) override
	{
		Scene::Update(dt);
		shader->Use();
		vertex_array->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		constexpr ImVec2 size{ 100,100 };
		ImGui::Begin("test");
		
		unsigned textureID = test_texture->GetTextureID();
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		ImGui::End();
	}
	void Draw() override {}
	void Unload()  override {}
private:

};

TestScene::TestScene(std::string t):Scene(),text(t)
{
	//components->AddComponent<A>(A{});
	//components->AddComponent<B>(B{});

}

TestScene::~TestScene()
{
}