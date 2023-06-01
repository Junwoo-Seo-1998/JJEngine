#pragma once
#include "Core/Scene.h"
#include <string>
#include "glad.h"
#include "Core/Graphics/Shader.h"
#include "Core/Graphics/VertexArray.h"
#include "Core/Graphics/VertexBuffer.h"
#include "Core/Graphics/FrameBuffer.h"
#include "Core/Graphics/IndexBuffer.h"
// for testing component manager

class TestScene : public Scene
{
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
	void Start() override;
	void Update() override;
private:

};

