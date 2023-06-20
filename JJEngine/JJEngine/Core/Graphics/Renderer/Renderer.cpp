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
 
static RenderCommandType command{};
static std::vector<glm::mat4> toLightVP;
static std::vector<LightInfo> lights;


static std::vector<ModelInfo> modelList;




void Renderer::BeginScene(const glm::mat4& viewProjection, const glm::vec3& camPos)
{
	command["toVP"] = viewProjection;
	command["camPos"] = camPos;
}

void Renderer::AddModel(const Model& model, const TransformComponent& transform, const MaterialComponent& material)
{
	if (material.type == MaterialType::Deferred) command["DefferedShader"] = material.defferedSecondPassShader;
	modelList.push_back(ModelInfo{ model, transform.GetTransform(), material });
}

void Renderer::AddAffectLight(const LightComponent& light, TransformComponent lightTransform)
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

void Renderer::SetVAO(std::shared_ptr<VertexArray> VAO)
{
	command["VAO"] = VAO;
}

void Renderer::SetGBuffer(std::shared_ptr<FrameBuffer> FBO, Mesh FSQ)
{
	command["GBufferFBO"] = FBO;
	command["FSQ"] = FSQ;

}

void Renderer::SetShadowBuffer(const std::shared_ptr<FrameBuffer>& FBO)
{
	command["ShadowMapFBO"] = FBO;
}

void Renderer::SetShadowInformation(glm::ivec2 resolution, glm::ivec2 zOffset)
{
	command["Shadow Resolution"] = resolution;
	command["Polygon Offset"] = zOffset;
}


void Renderer::EndScene()
{

	command["Lights"] = lights;
	command["Models"] = modelList;
		
	
	Graphics::GetInstance()->AddRenderCommand(command);

	lights.clear();
	modelList.clear();
	command.clear();
}

void Renderer::DrawAllScene()
{
	Graphics::GetInstance()->ExecuteRenderCommands();
}

