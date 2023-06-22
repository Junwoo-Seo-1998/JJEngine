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

struct RendererData
{
	std::shared_ptr<RenderPass> m_ActiveRenderPass;

	//TODO
	/*
	std::shared_ptr<VertexBuffer> m_FullscreenQuadVertexBuffer;
	std::shared_ptr<IndexBuffer> m_FullscreenQuadIndexBuffer;
	std::shared_ptr<Pipeline> m_FullscreenQuadPipeline;*/
};

static RendererData s_Data;


void Renderer::BeginRenderPass(std::shared_ptr<RenderPass> renderPass, bool clear)
{
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
	ENGINE_ASSERT(s_Data.m_ActiveRenderPass, "No active render pass!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	s_Data.m_ActiveRenderPass = nullptr;
}


