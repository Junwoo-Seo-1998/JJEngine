#include "Renderer2D.h"
#include "Core/Graphics/VertexArray.h"
#include "Core/Graphics/VertexBuffer.h"
#include "Core/Graphics/Shader.h"
#include "Core/Graphics/Texture.h"
#include <memory>
#include <array>

#include "EditorCamera.h"
#include "glad.h"
#include "Renderer.h"
#include "Core/Graphics/IndexBuffer.h"

struct QuadVertex
{
	glm::vec3 Position;
	glm::vec2 UV;
};

struct Renderer2DData
{
	std::shared_ptr<VertexArray> QuadVertexArray;
	std::shared_ptr<VertexBuffer> QuadVertexBuffer;
	std::shared_ptr<IndexBuffer> QuadIndexBuffer;
	std::shared_ptr<Shader>TextureShader;
	std::shared_ptr<Shader>EntityIDShader;

	std::shared_ptr<Texture> WhiteTexture;

	QuadVertex QuadVertices[4];
};

static Renderer2DData s_Data;

void Renderer2D::Init()
{
	s_Data.QuadVertexArray = VertexArray::CreateVertexArray();

	s_Data.QuadVertexArray->Bind();

	s_Data.QuadVertexBuffer = VertexBuffer::CreateVertexBuffer(sizeof(QuadVertex) * 4);

	s_Data.QuadVertices[0] = { {-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f} };
	s_Data.QuadVertices[1] = { {0.5f, -0.5f, 0.0f}, { 1.0f, 0.0f} };
	s_Data.QuadVertices[2] = { {0.5f,  0.5f, 0.0f}, {1.0f, 1.0f} };
	s_Data.QuadVertices[3] = { {-0.5f,  0.5f, 0.0f}, { 0.0f, 1.0f} };

	s_Data.QuadVertexBuffer->SetData(sizeof(QuadVertex) * 4, &s_Data.QuadVertices[0]);
	s_Data.QuadVertexBuffer->SetDataTypes({
			{0, DataType::Float3},//location=0, pos
			{1, DataType::Float2},//location=1, uv
		});

	s_Data.QuadVertexBuffer->BindToVertexArray();

	{
		//for later use
		std::shared_ptr<uint32_t[]> quadIndices(new uint32_t[6]);
		uint32_t offset = 0;
		for (uint32_t i = 0; i < 6; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		
		s_Data.QuadIndexBuffer = IndexBuffer::CreateIndexBuffer(6 * sizeof(uint32_t));
		s_Data.QuadIndexBuffer->SetData(6 * sizeof(uint32_t), &quadIndices[0]);
		s_Data.QuadIndexBuffer->BindToVertexArray();
	}

	s_Data.WhiteTexture = Renderer::WhiteTexture;
	s_Data.WhiteTexture->BindTexture();
	s_Data.TextureShader = Shader::CreateShaderFromFile({
		{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/simple.vert"}},
		{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/simple.frag"} }
	});

	s_Data.EntityIDShader = Shader::CreateShaderFromFile({
		{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/EntityID.vert"}},
		{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/EntityID.frag"} }
	});
	
}

void Renderer2D::Shutdown()
{
}

void Renderer2D::BeginScene(const glm::mat4& viewProjection)
{
	s_Data.TextureShader->Use();
	s_Data.TextureShader->SetMat4("ViewProjection", viewProjection);

	s_Data.QuadVertexArray->Bind();
	s_Data.WhiteTexture->BindTexture();

	glDisable(GL_CULL_FACE);
	
}

void Renderer2D::BeginScene(const EditorCamera& camera)
{
	s_Data.TextureShader->Use();
	s_Data.TextureShader->SetMat4("ViewProjection", camera.GetViewProjection());

	s_Data.QuadVertexArray->Bind();
	s_Data.WhiteTexture->BindTexture();
}


void Renderer2D::EndScene()
{
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
{
	s_Data.TextureShader->SetMat4("Transform", transform);
	s_Data.TextureShader->SetFloat4("Color", color);
	s_Data.TextureShader->SetInt("TextureUnit", 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture>& texture,
	const glm::vec4& tintColor)
{
	s_Data.TextureShader->SetMat4("Transform", transform);
	s_Data.TextureShader->SetFloat4("Color", tintColor);
	texture->BindTexture(1);
	s_Data.TextureShader->SetInt("TextureUnit", 1);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Renderer2D::BeginSceneEntityID(const EditorCamera& camera)
{
	s_Data.EntityIDShader->Use();
	s_Data.EntityIDShader->SetMat4("ViewProjection", camera.GetViewProjection());

	s_Data.QuadVertexArray->Bind();
}

void Renderer2D::EndSceneEntityID()
{
}

void Renderer2D::DrawQuadEntityID(const glm::mat4& transform, int entityID)
{
	s_Data.EntityIDShader->SetMat4("Transform", transform);
	s_Data.EntityIDShader->SetInt("EntityID", entityID);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
