#include "Renderer.h"
#include "Core/Graphics/VertexArray.h"
#include "Core/Graphics/VertexBuffer.h"
#include "Core/Graphics/Shader.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Utils/Math/MatrixMath.h"

#include <memory>
#include <array>

#include "glad.h"
#include "Core/Graphics/IndexBuffer.h"
#include "Core/Graphics/RenderCommand.h"

#include "Core/Graphics/RenderPass.h"
#include "Core/Utils/Assert.h"
#include "Core/Utils/File.h"

struct QuadVertex
{
	glm::vec3 Position;
	glm::vec2 UV;
};

struct RendererData
{
	std::shared_ptr<RenderPass> m_ActiveRenderPass;

	std::shared_ptr<Shader> m_FullScreenQuadShader;

	std::shared_ptr<VertexBuffer> m_FullScreenQuadVertexBuffer;
	std::shared_ptr<IndexBuffer> m_FullScreenQuadIndexBuffer;

	//todo: implement this 
	//we don't use it yet
	//std::shared_ptr<Pipeline> m_FullscreenQuadPipeline;
};

static RendererData s_Data;

//needs to be accessed from template function
CommandQueue Renderer::m_RenderCommandQueue;


void Renderer::BeginRenderPass(std::shared_ptr<RenderPass> renderPass, bool clear)
{
	//DEBUG_ONLY(EngineLog::Info("Begin RenderPass : {}",renderPass->GetSpecification().DebugName));
	ENGINE_ASSERT(renderPass, "Render pass cannot be nullptr!");
	s_Data.m_ActiveRenderPass = renderPass;
	renderPass->GetSpecification().TargetFramebuffer->Bind();
	if (clear)
	{
		const glm::vec4& clearColor = renderPass->GetSpecification().TargetFramebuffer->GetSpecification().ClearColor;
		RenderCommand::SetClearColor(clearColor);
		RenderCommand::Clear();
	}
}

void Renderer::EndRenderPass()
{
	//DEBUG_ONLY(EngineLog::Info("End RenderPass : {}", s_Data.m_ActiveRenderPass->GetSpecification().DebugName));
	ENGINE_ASSERT(s_Data.m_ActiveRenderPass, "No active render pass!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	s_Data.m_ActiveRenderPass = nullptr;
}

void Renderer::DrawFullScreenQuad()
{
	//you should set shader out side of this function

	s_Data.m_FullScreenQuadVertexBuffer->BindToVertexArray();
	s_Data.m_FullScreenQuadIndexBuffer->BindToVertexArray();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Renderer::Render()
{
	m_RenderCommandQueue.Execute();
}

void Renderer::Init()
{
	{//set vertex
		QuadVertex QuadVertices[4];
		QuadVertices[0] = { {-1.f, -1.f, 0.0f}, {0.0f, 0.0f} };//bottom left
		QuadVertices[1] = { {1.f, -1.f, 0.0f}, { 1.0f, 0.0f} };//bottom right
		QuadVertices[2] = { {1.f,  1.f, 0.0f}, {1.0f, 1.0f} };//top right
		QuadVertices[3] = { {-1.f,  1.f, 0.0f}, { 0.0f, 1.0f} };//top left

		s_Data.m_FullScreenQuadVertexBuffer = VertexBuffer::CreateVertexBuffer(sizeof(QuadVertex) * 4);
		s_Data.m_FullScreenQuadVertexBuffer->SetData(sizeof(QuadVertex) * 4, &QuadVertices[0]);
		s_Data.m_FullScreenQuadVertexBuffer->SetDataTypes({
				{0, DataType::Float3},//location=0, pos
				{1, DataType::Float2},//location=1, uv
			});
	}

	{//set index
		uint32_t quadIndices[6];
		quadIndices[0] = 0;
		quadIndices[1] = 1;
		quadIndices[2] = 2;

		quadIndices[3] = 2;
		quadIndices[4] = 3;
		quadIndices[5] = 0;

		s_Data.m_FullScreenQuadIndexBuffer = IndexBuffer::CreateIndexBuffer(6 * sizeof(uint32_t));
		s_Data.m_FullScreenQuadIndexBuffer->SetData(6 * sizeof(uint32_t), &quadIndices[0]);
	}
}


