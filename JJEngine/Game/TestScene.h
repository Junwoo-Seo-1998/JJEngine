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
	using Entity::Entity;
	void Update(double )override { std::cout << "A" << std::endl; }
};
class B : public Entity {
	short b{2};
public:
	using Entity::Entity;

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

		float textureQuad[] = {
			// positions        // texture coords
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // top right
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // bottom left
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,    // top left 
			- 0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // bottom left
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // top right
		};

		vertex_array = VertexArray::CreateVertexArray();
		vertex_array->Bind();
		vertex_buffer = VertexBuffer::CreateVertexBuffer(sizeof(textureQuad));
		vertex_buffer->SetData(sizeof(textureQuad), textureQuad);
		vertex_buffer->SetDataTypes({ 
			{0, DataType::Float3},//location=0, pos
			{1, DataType::Float2},//location=1, uv
		});
		vertex_buffer->BindToVertexArray();

		//vertex_color_buffer = VertexBuffer::CreateVertexBuffer(sizeof(colors));
		//vertex_color_buffer->SetData(sizeof(colors), colors);
		//vertex_color_buffer->SetDataTypes({ {1,DataType::Float3} });
		//vertex_color_buffer->BindToVertexArray();
		

		test_texture = Texture::CreateTexture(File::ReadImageToTexture("Resources/Textures/test.jpg"));
	};
	void Update(double dt) override
	{

		Scene::Update(dt);
		//draw test
		shader->Use();
		vertex_array->Bind();
		shader->SetInt("testTexture", 0);
		test_texture->BindTexture();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//for imgui test
		ImGui::Begin("test");
		constexpr ImVec2 size{ 100,100 };
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
	ID Aid =  components->GetNewEntityID();
	ID Bid = components->GetNewEntityID();

	//components->AddComponent<A>(Aid,A{ components,Aid });
	//components->AddComponent<B>(Bid,B{ components,Bid });

}

TestScene::~TestScene()
{
}