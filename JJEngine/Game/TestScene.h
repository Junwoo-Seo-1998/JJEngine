#pragma once
#include "Core/Scene.h"
#include <string>
#include <iostream>

#include "glad.h"
#include "Core/Graphics/VertexArray.h"

class TestScene: public Scene
{
	std::string text{};
public:
	TestScene(std::string);
	~TestScene();
	void Load() override{ std::cout << text << std::endl; };
	void Update(double) override
	{
		auto shader = Shader::CreateShaderFromFile({
	{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/simple.vert"}},
	{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/simple.frag"} }
			});
		shader->Use();
		//DescribedData te{ {0,DataType::Float3} };
		//DescribedDataBlock test{{{0,DataType::Float3}},{{0,DataType::Float3}},{{0,DataType::Float3}}};
		float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
		};
		auto vertex_array = VertexArray::CreateVertexArray();
		vertex_array->Bind();
		auto vertex_buffer = VertexBuffer::CreateVertexBuffer(1024);
		vertex_buffer->SetData(sizeof(vertices), vertices);
		vertex_buffer->Bind();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	void Draw() override {}
	void Unload()  override {}
private:

};

TestScene::TestScene(std::string t):Scene(),text(t)
{
}

TestScene::~TestScene()
{
}