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
 
static RenderCommand command{};
static std::vector<glm::mat4> toLightVP;
static std::vector<LightInfo> lights;
 



void Renderer::BeginScene(const glm::mat4& viewProjection)
{
	command.variables["toVP"] = viewProjection;
}

void Renderer::SetModel(const Model& model)
{
	command.variables["Model"] = model;
}

void Renderer::AddAffectLight(const LightComponent& light, TransformComponent lightTransform)
{
	switch (light.type)
	{
	case LightType::Point:
	{
		lightTransform.LookAtDir({1.f, 0.f, 0.f});
		toLightVP.push_back( light.GetProjection() * MatrixMath::BuildCameraMatrix(lightTransform.Position, lightTransform.GetForward(), lightTransform.GetUp()));
		lightTransform.LookAtDir({ -1.f, 0.f, 0.f });
		toLightVP.push_back( light.GetProjection() * MatrixMath::BuildCameraMatrix(lightTransform.Position, lightTransform.GetForward(), lightTransform.GetUp()));
		lightTransform.LookAtDir({ 0.f, 1.f, 0.f });
		toLightVP.push_back( light.GetProjection() * MatrixMath::BuildCameraMatrix(lightTransform.Position, lightTransform.GetForward(), lightTransform.GetUp()));
		lightTransform.LookAtDir({ 0.f, -1.f, 0.f });
		toLightVP.push_back( light.GetProjection() * MatrixMath::BuildCameraMatrix(lightTransform.Position, lightTransform.GetForward(), lightTransform.GetUp()));
		lightTransform.LookAtDir({ 0.f, 0.f, 1.f });
		toLightVP.push_back( light.GetProjection() * MatrixMath::BuildCameraMatrix(lightTransform.Position, lightTransform.GetForward(), lightTransform.GetUp()));
		lightTransform.LookAtDir({ 0.f, 0.f, -1.f });
		toLightVP.push_back( light.GetProjection() * MatrixMath::BuildCameraMatrix(lightTransform.Position, lightTransform.GetForward(), lightTransform.GetUp()));

		lights.push_back({ light.type, toLightVP , lightTransform.Position });
	}
	break;
	}
}

void Renderer::SetTransform(const TransformComponent& transform)
{
	command.variables["toWorld"] = transform.GetTransform();
}

void Renderer::SetMaterial(const MaterialComponent& material)
{
	command.commandType = material.isShadowed ? (material.type == MaterialType::Forward ? CommandType::ForwardShaded : CommandType::DefferedShaded) : (material.type == MaterialType::Forward ? CommandType::Forward : CommandType::Deffered);
	command.variables["ForwardShader"] = material.forwardShader;
	command.variables["DefferedFirstPassShader"] = material.defferedFirstPassShader;
	command.variables["DefferedSecondPassShader"] = material.defferedSecondPassShader;
	command.variables["ShadowSamplingShader"] = material.shadowSamplingShader;
}

void Renderer::SetVAO(std::shared_ptr<VertexArray> VAO)
{
	command.variables["VAO"] = VAO;
}

void Renderer::SetGBuffer(std::shared_ptr<FrameBuffer> FBO)
{
	command.variables["GBufferFBO"] = FBO;

}

void Renderer::SetShadowBuffer(const std::shared_ptr<FrameBuffer>& FBO)
{
	command.variables["ShadowMapFBO"] = FBO;
}


void Renderer::EndScene()
{
	command.variables["Lights"] = lights;
	Graphics::GetInstance()->AddRenderCommand(command.commandType, command.variables);

}

