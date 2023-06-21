#include "Renderer.h"
#include "Core/Graphics/VertexArray.h"
#include "Core/Graphics/VertexBuffer.h"
#include "Core/Graphics/Shader.h"
#include "Core/Graphics/Texture.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Utils/Math/MatrixMath.h"

#include <memory>
#include <array>

#include "glad.h"
#include "Core/Graphics/IndexBuffer.h"
#include "Core/Graphics/RenderCommand.h"

#include "Core/Graphics/RenderPass.h"
#include "Core/Utils/Assert.h"

static RenderCommandType command{};
static std::vector<glm::mat4> toLightVP;
static std::vector<LightInfo> lights;


static std::vector<ModelInfo> modelList;

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

void SceneRenderer::BeginScene(const glm::mat4& viewProjection, const glm::vec3& camPos)
{
	command["toVP"] = viewProjection;
	command["camPos"] = camPos;
}

void SceneRenderer::AddModel(const Model& model, const TransformComponent& transform, const MaterialComponent& material)
{
	if (material.type == MaterialType::Deferred) command["DefferedShader"] = material.defferedSecondPassShader;
	modelList.push_back(ModelInfo{ model, transform.GetTransform(), material });
}

void SceneRenderer::AddAffectLight(const LightComponent& light, TransformComponent lightTransform)
{
	switch (light.type)
	{
	case LightType::Point:
	{

		toLightVP.push_back(light.GetProjection() * MatrixMath::BuildCameraMatrixWithDirection(lightTransform.Position, { 1.f, 0.f, 0.f }));
		toLightVP.push_back(light.GetProjection() * MatrixMath::BuildCameraMatrixWithDirection(lightTransform.Position, { -1.f, 0.f, 0.f }));
		toLightVP.push_back(light.GetProjection() * MatrixMath::BuildCameraMatrixWithDirection(lightTransform.Position, { 0.f, 1.f, 0.f }, {0, 0, 1}));
		toLightVP.push_back(light.GetProjection() * MatrixMath::BuildCameraMatrixWithDirection(lightTransform.Position, { 0.f, -1.f, 0.f }, {0, 0, -1}));
		toLightVP.push_back(light.GetProjection() * MatrixMath::BuildCameraMatrixWithDirection(lightTransform.Position, { 0.f, 0.f, 1.f }));
		toLightVP.push_back(light.GetProjection() * MatrixMath::BuildCameraMatrixWithDirection(lightTransform.Position, { 0.f, 0.f, -1.f }));

		lights.push_back({ light.type, toLightVP , lightTransform.Position });
		toLightVP.clear();
	}
	break;
	}
}

void SceneRenderer::SetVAO(std::shared_ptr<VertexArray> VAO)
{
	command["VAO"] = VAO;
}

void SceneRenderer::SetGBuffer(std::shared_ptr<FrameBuffer> FBO, Mesh FSQ)
{
	command["GBufferFBO"] = FBO;
	command["FSQ"] = FSQ;

}

void SceneRenderer::SetShadowBuffer(const std::shared_ptr<FrameBuffer>& FBO)
{
	command["ShadowMapFBO"] = FBO;
}

void SceneRenderer::SetShadowInformation(glm::ivec2 resolution, glm::ivec2 zOffset)
{
	command["Shadow Resolution"] = resolution;
	command["Polygon Offset"] = zOffset;
}


void SceneRenderer::EndScene()
{
	command["Lights"] = lights;
	command["Models"] = modelList;
		
	
	Graphics::GetInstance()->AddRenderCommand(command);

	lights.clear();
	modelList.clear();
	command.clear();
}

void SceneRenderer::DrawAllScene()
{
	Graphics::GetInstance()->ExecuteRenderCommands();
}

