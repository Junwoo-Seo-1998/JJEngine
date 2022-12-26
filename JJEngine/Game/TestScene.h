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
public:
	TestScene(std::string);
	~TestScene();
	void Load() override{ std::cout << text << std::endl; };
	void Update(double dt) override
	{
		Scene::Update(dt);
		auto shader = Shader::CreateShaderFromFile({
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
		
		auto vertex_array = VertexArray::CreateVertexArray();
		vertex_array->Bind();
		auto vertex_buffer = VertexBuffer::CreateVertexBuffer(sizeof(vertices));
		vertex_buffer->SetData(sizeof(vertices), vertices);
		vertex_buffer->SetDataTypes({ {0, DataType::Float3} });
		vertex_buffer->BindToVertexArray();

		auto vertex_color_buffer = VertexBuffer::CreateVertexBuffer(sizeof(colors));
		vertex_color_buffer->SetData(sizeof(colors), colors);
		vertex_color_buffer->SetDataTypes({ {1,DataType::Float3} });
		vertex_color_buffer->BindToVertexArray();
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	void Draw() override {}
	void Unload()  override {}
private:

};

TestScene::TestScene(std::string t):Scene(),text(t)
{
	ID Aid =  components->GetNewEntityID();
	ID Bid = components->GetNewEntityID();

	components->AddComponent<A>(Aid,A{ components,Aid });
	components->AddComponent<B>(Bid,B{ components,Bid });

}

TestScene::~TestScene()
{
}