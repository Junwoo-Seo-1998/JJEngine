#include "SceneRenderer.h"

#include "Renderer.h"
#include "Core/Scene.h"
#include "Core/Component/TransformComponent.h"
#include "Core/Component/LightComponent.h"
#include "Core/Component/MaterialComponent.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/RenderPass.h"
#include "Core/Graphics/FrameBuffer.h"
#include "Core/Graphics/Material.h"
#include "Core/Graphics/RenderCommand.h"
#include "Core/Utils/Assert.h"
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
	if(width > 0 && height > 0)
	{
		m_Width = width;
		m_Height = height;
		m_NeedsResize = true;
	}
}

void SceneRenderer::BeginScene(const glm::mat4& view, const glm::mat4& Projection, const glm::vec3& camPos)
{
	ENGINE_ASSERT(m_ActiveScene != nullptr, "Scene is nullptr");
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

	{//collect lights
		auto view = m_ActiveScene->GetRegistry().view<TransformComponent, LightComponent>();
		for (auto entity: view)
		{
			auto [transform, light] = view.get<TransformComponent, LightComponent>(entity);
			m_ActiveLights.emplace_back(std::tuple{ transform.Position, transform.GetForward(), light.light });
		}

		if(m_ActiveLights.empty())
		{
			EngineLog::Warn("There is no light");
		}
	}
}

void SceneRenderer::EndScene()
{
	ENGINE_ASSERT(m_ActiveScene != nullptr, "Scene is nullptr");
	Renderer::Submit([this]() {m_VertexArray->Bind(); });

	GeometryPass();

	GeometryPassFSQ();

	//real rendering happens here
	Renderer::Render();

	m_DrawList.clear();
	m_ActiveLights.clear();

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

		m_FinalRenderShader = Shader::CreateShaderFromFile({
		{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/FSQShader.vert"}},
		{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/FSQShader.frag"} }
		});
	}

	{//setting up default material
		m_DefaultMaterial = Material::Create(m_GeometryShader);
		m_DefaultMaterial->Set("MatTexture.Diffuse", Texture::CreateTexture(glm::vec4{ 0.8f, 0.8f, 0.8f, 1.f }));
		m_DefaultMaterial->Set("MatTexture.Specular", Texture::CreateTexture(glm::vec4{ 0.5f, 0.5f, 0.5f, 1.f }));
		m_DefaultMaterial->Set("MatTexture.Emissive", Renderer::BlackTexture);
		m_DefaultMaterial->Set("MatTexture.Shininess", 32.0f);
	}

	{//geo
		RenderPassSpecification spec;
		spec.DebugName = "GeometryRenderPass";
		FrameBufferSpecification fb_spec;
		//should keep black!!
		fb_spec.ClearColor = { 0.0f, 0.0f, 0.0f,0.f };
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
		fb_spec.ClearColor = { 0.3,0.3,0.3,1.f };
		fb_spec.Width = 400;
		fb_spec.Height = 400;
		fb_spec.Formats = { FrameBufferFormat::RGBA, FrameBufferFormat::Depth };
		spec.TargetFramebuffer = FrameBuffer::CreateFrameBuffer(fb_spec);
		m_FinalRenderPass = RenderPass::Create(spec);
	}
}

void SceneRenderer::GeometryPass()
{
	Renderer::Submit([this]()
	{
		Renderer::BeginRenderPass(m_GeometryRenderPass);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	});

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
			m_GeometryShader->SetMat4("Matrix.Normal", glm::transpose(glm::inverse(toDraw.Transform)));
			m_DefaultMaterial->Bind();
			//todo:draw it material instance
			//toDraw.Mesh->GetMaterial()->Bind();
			glDrawElements(GL_TRIANGLES, toDraw.Mesh->GetNumOfIndices(), GL_UNSIGNED_INT, nullptr);
		});
	}
	Renderer::Submit([]() {Renderer::EndRenderPass(); });
}

void SceneRenderer::GeometryPassFSQ()
{
	Renderer::Submit([this]()
	{//draw result of geometry pass
		Renderer::BeginRenderPass(m_FinalRenderPass, true);

		{
			m_FinalRenderShader->Use();

			//bind gbuffer
			{
				m_FinalRenderShader->SetInt("gPosition", 0);
				m_FinalRenderShader->SetInt("gNormal", 1);
				m_FinalRenderShader->SetInt("gDiffuse", 2);
				m_FinalRenderShader->SetInt("gSpecular", 3);
				m_FinalRenderShader->SetInt("gEmissive", 4);

				auto fb = m_GeometryRenderPass->GetSpecification().TargetFramebuffer;

				fb->GetColorTexture(0)->BindTexture(0);
				fb->GetColorTexture(1)->BindTexture(1);
				fb->GetColorTexture(2)->BindTexture(2);
				fb->GetColorTexture(3)->BindTexture(3);
				fb->GetColorTexture(4)->BindTexture(4);
			}

			m_FinalRenderShader->SetFloat3("CameraPosition", m_CameraPosition);

			//light set up
			{
				m_FinalRenderShader->SetInt("LightNumbers", m_ActiveLights.size());
				int index = 0;
				for (auto& [lightPosition, lightDir, light] : m_ActiveLights)
				{
					std::string lightIndex = std::format("Light[{}].", index++);

					m_FinalRenderShader->SetInt(lightIndex + "LightType", static_cast<int>(light.m_LightType));
					m_FinalRenderShader->SetFloat3(lightIndex + "Position", lightPosition);
					m_FinalRenderShader->SetFloat3(lightIndex + "Direction", lightDir);
					m_FinalRenderShader->SetFloat(lightIndex + "InnerAngle", light.m_Angle.inner);
					m_FinalRenderShader->SetFloat(lightIndex + "OuterAngle", light.m_Angle.outer);
					m_FinalRenderShader->SetFloat(lightIndex + "FallOff", light.falloff);
					m_FinalRenderShader->SetFloat3(lightIndex + "Ambient", light.Ambient);
					m_FinalRenderShader->SetFloat3(lightIndex + "Diffuse", light.Diffuse);
					m_FinalRenderShader->SetFloat3(lightIndex + "Specular", light.Specular);
				}
			}

			//hard coded for now
			{
				m_FinalRenderShader->SetFloat3("globalAmbient", {});

				m_FinalRenderShader->SetFloat("Attenuation.c1", 1.f);
				m_FinalRenderShader->SetFloat("Attenuation.c2", 0.027f);
				m_FinalRenderShader->SetFloat("Attenuation.c3", 0.0028f);

				m_FinalRenderShader->SetFloat3("Fog.Color", { 0.5f,0.5f,0.5f });
				m_FinalRenderShader->SetFloat("Fog.Near", 1.f);
				m_FinalRenderShader->SetFloat("Fog.Far", 500000.f);
			}

			//vbo and ibo is provided inside of this func 
			Renderer::DrawFullScreenQuad();
		}

		Renderer::EndRenderPass();

		//copy depth
		{
			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GeometryRenderPass->GetSpecification().TargetFramebuffer->GetFrameBufferID());
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FinalRenderPass->GetSpecification().TargetFramebuffer->GetFrameBufferID());
			glBlitFramebuffer(
				0, 0, m_Width, m_Height, // source region
				0, 0, m_Width, m_Height, // destination region
				GL_DEPTH_BUFFER_BIT, // field to copy
				GL_NEAREST // filtering mechanism
			);
		}
	});
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
	case LightType::PointLight:
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

