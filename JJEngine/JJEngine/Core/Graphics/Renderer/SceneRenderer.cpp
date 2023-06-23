#include "SceneRenderer.h"

#include "Renderer.h"
#include "Core/Component/TransformComponent.h"
#include "Core/Component/LightComponent.h"
#include "Core/Component/MaterialComponent.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/RenderPass.h"
#include "Core/Graphics/FrameBuffer.h"
#include "Core/Graphics/Material.h"
#include "Core/Graphics/RenderCommand.h"
#include "Core/Utils/File.h"
#include "Core/Utils/Math/MatrixMath.h"

static RenderCommandType command{};
static std::vector<glm::mat4> toLightVP;
static std::vector<LightInfo> lights;
static std::vector<ModelInfo> modelList;

SceneRenderer::SceneRenderer()
{
	Init();
}

void SceneRenderer::SetScene(Scene* scene)
{
	m_ActiveScene = scene;
}

void SceneRenderer::SetViewportSize(unsigned width, unsigned height)
{
	if(width > 0.0f && height > 0.0f)
	{
		m_Width = width;
		m_Height = height;
		m_NeedsResize = true;
	}
}

void SceneRenderer::BeginScene(const glm::mat4& view, const glm::mat4& Projection, const glm::vec3& camPos)
{
	m_Active = true;
	if(m_NeedsResize)
	{
		m_GeometryRenderPass->GetSpecification().TargetFramebuffer->Resize(m_Width, m_Height);
		m_FinalRenderPass->GetSpecification().TargetFramebuffer->Resize(m_Width, m_Height);
		m_NeedsResize = false;
	}

	//it will be removed later // get it from scene
	m_View = view;
	m_Projection = Projection;
	m_CameraPosition = camPos;
}

void SceneRenderer::EndScene()
{
	Renderer::Submit([this]() {m_VertexArray->Bind(); });

	GeometryPass();

	Renderer::Submit([this]() {Renderer::BeginRenderPass(m_FinalRenderPass, true); });
	{//submit final fsq
		


	}
	Renderer::Submit([this]() {	Renderer::EndRenderPass(); });

	Renderer::Render();

	m_DrawList.clear();
	m_Active = false;
}

void SceneRenderer::SubmitMesh(std::shared_ptr<Mesh> mesh, const glm::mat4& transformMat)
{
	//todo: make user defined draw list later
	m_DrawList.emplace_back(mesh, transformMat);
}

std::shared_ptr<RenderPass> SceneRenderer::GetFinalRenderPass()
{
	return m_FinalRenderPass;
}

void SceneRenderer::Init()
{
	m_VertexArray = VertexArray::CreateVertexArray();

	m_Width = 400;
	m_Height = 400;

	{//setting up shader 
		m_GeometryShader = Shader::CreateShaderFromFile({
		{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/GBufferShader.vert"}},
		{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/GBufferShader.frag"} }
		});
	}

	{//setting up default material
		m_DefaultMaterial = Material::Create(m_GeometryShader);
		m_DefaultMaterial->Set("MatTexture.Diffuse", Texture::CreateTexture(File::ReadImageToTexture("Resources/Textures/TestDiff.png")));
		m_DefaultMaterial->Set("MatTexture.Specular", Texture::CreateTexture(File::ReadImageToTexture("Resources/Textures/TestSpec.png")));
		m_DefaultMaterial->Set("MatTexture.Emissive", Texture::CreateTexture(File::ReadImageToTexture("Resources/Textures/TestSpec.png")));
		m_DefaultMaterial->Set("MatTexture.Shininess", 2.0f);
	}

	{//geo
		RenderPassSpecification spec;
		spec.DebugName = "Final Render";
		FrameBufferSpecification fb_spec;
		fb_spec.ClearColor = { 0,0,0,1.f };
		fb_spec.Width = 400;
		fb_spec.Height = 400;
		fb_spec.Formats = {
			FrameBufferFormat::RGBA32F, FrameBufferFormat::RGBA32F, //gPosition, gNormal 
			FrameBufferFormat::RGBA32F, FrameBufferFormat::RGBA32F, //gDiffuse, gSpecular
			FrameBufferFormat::RGBA32F, FrameBufferFormat::Depth //gEmissive, Depth
		};
		spec.TargetFramebuffer = FrameBuffer::CreateFrameBuffer(fb_spec);
		m_GeometryRenderPass = RenderPass::Create(spec);
	}

	{//final
		RenderPassSpecification spec;
		spec.DebugName = "Final Render";
		FrameBufferSpecification fb_spec;
		fb_spec.ClearColor = { 0,0,0,1.f };
		fb_spec.Width = 400;
		fb_spec.Height = 400;
		fb_spec.Formats = { FrameBufferFormat::RGBA, FrameBufferFormat::Depth };
		spec.TargetFramebuffer = FrameBuffer::CreateFrameBuffer(fb_spec);
		m_FinalRenderPass = RenderPass::Create(spec);
	}
}

void SceneRenderer::GeometryPass()
{
	Renderer::Submit([this]() {Renderer::BeginRenderPass(m_GeometryRenderPass); });

	Renderer::Submit([this]()
	{
		m_GeometryShader->Use();
		m_GeometryShader->SetMat4("Matrix.View", m_View);
		m_GeometryShader->SetMat4("Matrix.Projection", m_Projection);
	});

	//we should check time it will be faster to run the loop inside of submit or call it with small chunk
	//since lambda could be faster when it's small
	for (auto& toDraw : m_DrawList)
	{
		Renderer::Submit([this, &toDraw]()
		{
			toDraw.Mesh->GetMeshVBO()->BindToVertexArray();
			toDraw.Mesh->GetMeshEBO()->BindToVertexArray();

			m_GeometryShader->SetMat4("Matrix.Model", toDraw.Transform);
			m_DefaultMaterial->Bind();
			//todo:draw it material instance
			//toDraw.Mesh->GetMaterial()->Bind();
			glDrawElements(GL_TRIANGLES, toDraw.Mesh->GetNumOfIndices(), GL_UNSIGNED_INT, nullptr);
		});
	}
	Renderer::Submit([]() {Renderer::EndRenderPass(); });
}
	

void SceneRenderer::BeginSceneCommand(const glm::mat4& viewProjection, const glm::vec3& camPos)
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
		toLightVP.push_back(light.GetProjection() * MatrixMath::BuildCameraMatrixWithDirection(lightTransform.Position, { 0.f, 1.f, 0.f }, { 0, 0, 1 }));
		toLightVP.push_back(light.GetProjection() * MatrixMath::BuildCameraMatrixWithDirection(lightTransform.Position, { 0.f, -1.f, 0.f }, { 0, 0, -1 }));
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


void SceneRenderer::EndSceneCommand()
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

