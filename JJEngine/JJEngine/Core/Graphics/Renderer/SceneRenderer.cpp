#include "SceneRenderer.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Renderer.h"
#include "Core/Scene.h"
#include "Core/Component/TransformComponent.h"
#include "Core/Component/LightComponent.h"
#include "Core/Component/MaterialComponent.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/RenderPass.h"
#include "Core/Graphics/FrameBuffer.h"
#include "Core/Graphics/MeshFactory.h"
#include "Core/Graphics/Material.h"
#include "Core/Graphics/RenderCommand.h"

#include "Core/Utils/Assert.h"
#include "Core/Utils/File.h"
#include "Core/Utils/Math/MatrixMath.h"
#include "imgui.h"

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
	if (width > 0 && height > 0)
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
	if (m_NeedsResize)
	{
		m_GeometryRenderPass->GetSpecification().TargetFramebuffer->Resize(m_Width, m_Height);
		m_FinalRenderPass->GetSpecification().TargetFramebuffer->Resize(m_Width, m_Height);

		//bloom
		m_BloomExtractRenderPass->GetSpecification().TargetFramebuffer->Resize(m_Width, m_Height);
		m_BloomBlurRenderPass[0]->GetSpecification().TargetFramebuffer->Resize(m_Width, m_Height);
		m_BloomBlurRenderPass[1]->GetSpecification().TargetFramebuffer->Resize(m_Width, m_Height);

		m_NeedsResize = false;
	}

	//it will be removed later // get it from scene
	m_View = view;
	m_Projection = Projection;
	m_CameraPosition = camPos;


	{//collect lights
		auto view = m_ActiveScene->GetRegistry().view<TransformComponent, LightComponent>();
		int index = 0;
		for (auto entity : view)
		{
			auto [transform, light] = view.get<TransformComponent, LightComponent>(entity);
			m_ActiveLights.push_back(std::tuple{ transform.Position, transform.GetForward(), light.light });
		}

		if (m_ActiveLights.empty())
		{
			EngineLog::Warn("There is no light");
		}
	}
}

void SceneRenderer::EndScene()
{
	ENGINE_ASSERT(m_ActiveScene != nullptr, "Scene is nullptr");
	Renderer::Submit([this]() {m_VertexArray->Bind(); });

	//jun: it's capturing at every loop but it will be removed later after the asset work
	BakeCubeMap();

	//GeometryPass();

	//GeometryPassFSQ();

	ForwardPass();

	CubemapPass();
	//post processing
	BloomPass();
	HDRPass();

	//todo: make flags for it
	//DebugRenderingPass();

	//real rendering happens here
	Renderer::Render();

	m_DrawList.clear();
	m_GeometryDrawList.clear();
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

		m_NormalRenderShader = Shader::CreateShaderFromFile({
		{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/DebugNormal.vert"}},
		{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/DebugNormal.frag"}},
		{ ShaderType::GeometryShader,{"Resources/Shaders/version.glsl","Resources/Shaders/DebugNormal.geo"}}
			});

		m_ForwardRenderShader = Shader::CreateShaderFromFile({
		{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/ForwardShader.vert"}},
		{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/ForwardShader.frag"} }
			});

		m_GeometryShader = Shader::CreateShaderFromFile({
		{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/GBufferShader.vert"}},
		{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/GBufferShader.frag"} }
			});

		m_FinalRenderShader = Shader::CreateShaderFromFile({
		{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/FSQShader.vert"}},
		{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/FSQShader.frag"} }
			});

		m_BloomExtractShader = Shader::CreateShaderFromFile({
		{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/Bloom/BloomExtractShader.vert"}},
		{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/Bloom/BloomExtractShader.frag"} }
			});

		m_BloomBlurShader = Shader::CreateShaderFromFile({
		{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/Bloom/BloomBlurShader.vert"}},
		{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/Bloom/BloomBlurShader.frag"} }
			});

		m_BloomRenderShader = Shader::CreateShaderFromFile({
		{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/Bloom/BloomRenderShader.vert"}},
		{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/Bloom/BloomRenderShader.frag"} }
			});

		m_HDRRenderShader = Shader::CreateShaderFromFile({
		{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/HDRShader.vert"}},
		{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/HDRShader.frag"} }
			});
	}

	{//setting up default material
		m_DefaultMaterial = Material::Create(m_ForwardRenderShader);
		m_DefaultMaterial->Set("MatTexture.Diffuse", Texture::CreateTexture(glm::vec4{ 0.8f, 0.8f, 0.8f, 1.f }));
		m_DefaultMaterial->Set("MatTexture.Specular", Texture::CreateTexture(glm::vec4{ 0.5f, 0.5f, 0.5f, 1.f }));
		m_DefaultMaterial->Set("MatTexture.Emissive", Renderer::BlackTexture);
		m_DefaultMaterial->Set("MatTexture.Shininess", 12.0f);
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
		fb_spec.ClearColor = { 0.0,0.0,0.0,1.f };
		fb_spec.Width = 400;
		fb_spec.Height = 400;
		fb_spec.Formats = { FrameBufferFormat::RGBA32F, FrameBufferFormat::Depth };
		spec.TargetFramebuffer = FrameBuffer::CreateFrameBuffer(fb_spec);
		m_FinalRenderPass = RenderPass::Create(spec);
	}

	{//for bloom
		RenderPassSpecification spec;
		spec.DebugName = "Bloom Render";
		FrameBufferSpecification fb_spec;
		fb_spec.ClearColor = { 0.0, 0.0, 0.0,0.f };
		fb_spec.Width = 400;
		fb_spec.Height = 400;
		fb_spec.Formats = { FrameBufferFormat::RGBA32F, FrameBufferFormat::Depth };
		spec.TargetFramebuffer = FrameBuffer::CreateFrameBuffer(fb_spec);
		m_BloomExtractRenderPass = RenderPass::Create(spec);
		spec.TargetFramebuffer = FrameBuffer::CreateFrameBuffer(fb_spec);
		m_BloomBlurRenderPass[0] = RenderPass::Create(spec);
		spec.TargetFramebuffer = FrameBuffer::CreateFrameBuffer(fb_spec);
		m_BloomBlurRenderPass[1] = RenderPass::Create(spec);
	}

	{//cubemap variable initialization

		m_BakeCubeMapShader = Shader::CreateShaderFromFile({
		{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/HDRIConvertShader.vert"}},
		{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/HDRIConvertShader.frag"} }
			});

		m_CubeMapShader = Shader::CreateShaderFromFile({
		{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/CubemapShader.vert"}},
		{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/CubemapShader.frag"} }
			});

		m_DefaultHDRICubemapTexture = Texture::CreateTexture(File::ReadHDRImageToTexture("Resources/Textures/hdri_skybox.png"));

		m_CubeMapMesh = MeshFactory::CreateSkyCube({ 2.f, 2.f, 2.f });

		RenderPassSpecification spec;
		spec.DebugName = "Bake CubeMap";
		FrameBufferSpecification fb_spec;
		fb_spec.ClearColor = { 0.0, 0.0, 0.0,0.f };
		fb_spec.Width = 1024;
		fb_spec.Height = 1024;
		fb_spec.Formats = { FrameBufferFormat::CubeMap16F, FrameBufferFormat::Depth };
		spec.TargetFramebuffer = FrameBuffer::CreateFrameBuffer(fb_spec);
		m_BakeCubeMapRenderPass = RenderPass::Create(spec);
	}
}

void SceneRenderer::BakeCubeMap()
{

	Renderer::Submit([this]()
		{
			static const glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
			static const glm::mat4 captureViews[] =
			{
			   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
			};

			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				glDisable(GL_DEPTH_TEST);
			}

			Renderer::BeginRenderPass(m_BakeCubeMapRenderPass, true);
			m_BakeCubeMapShader->Use();
			m_BakeCubeMapShader->SetMat4("projection", captureProjection);
			m_DefaultHDRICubemapTexture->BindTexture(0);
			m_BakeCubeMapShader->SetInt("equirectangularMap", 0);
			for (unsigned int i = 0; i < 6; ++i)
			{
				m_BakeCubeMapShader->SetMat4("view", captureViews[i]);
				m_BakeCubeMapRenderPass->GetSpecification().TargetFramebuffer->ChangeCubeMapTextureFace(0, i);

				RenderCommand::Clear();

				m_CubeMapMesh->GetMeshVBO()->BindToVertexArray();
				m_CubeMapMesh->GetMeshEBO()->BindToVertexArray();
				glDrawElements(GL_TRIANGLES, m_CubeMapMesh->GetNumOfIndices(), GL_UNSIGNED_INT, nullptr);
			}
			Renderer::EndRenderPass();
		});
}

void SceneRenderer::CubemapPass()
{
	Renderer::Submit([this]()
		{
			Renderer::BeginRenderPass(m_FinalRenderPass, false);
		});

	Renderer::Submit([this]()
		{
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glDepthFunc(GL_LEQUAL);
			glDepthMask(GL_FALSE);
			glDisable(GL_BLEND);

			m_CubeMapShader->Use();
			glm::mat4 view = glm::mat4(glm::mat3(m_View));
			m_CubeMapShader->SetMat4("view", view);
			m_CubeMapShader->SetMat4("projection", m_Projection);
			m_BakeCubeMapRenderPass->GetSpecification().TargetFramebuffer->GetColorTexture(0)->BindTexture(0);
			m_CubeMapShader->SetInt("environmentMap", 0);
			m_CubeMapMesh->GetMeshVBO()->BindToVertexArray();
			m_CubeMapMesh->GetMeshEBO()->BindToVertexArray();
			glDrawElements(GL_TRIANGLES, m_CubeMapMesh->GetNumOfIndices(), GL_UNSIGNED_INT, nullptr);

			glDepthFunc(GL_LESS);
			glDepthMask(GL_TRUE);
		});


	Renderer::Submit([]()
		{
			Renderer::EndRenderPass();
		});
}

void SceneRenderer::GeometryPass()
{
	Renderer::Submit([this]()
		{
			Renderer::BeginRenderPass(m_GeometryRenderPass);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glDisable(GL_BLEND);
		});

	Renderer::Submit([this]()
		{
			m_GeometryShader->Use();
			m_GeometryShader->SetMat4("Matrix.View", m_View);
			m_GeometryShader->SetMat4("Matrix.Projection", m_Projection);
		});

	//we should check time it will be faster to run the loop inside of submit or call it with small chunk
	//since lambda could be faster when it's small
	for (auto& toDraw : m_GeometryDrawList)
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
					m_FinalRenderShader->SetFloat("Attenuation.c2", 0.22f);
					m_FinalRenderShader->SetFloat("Attenuation.c3", 0.20f);

					m_FinalRenderShader->SetFloat3("Fog.Color", { 0.5f,0.5f,0.5f });
					m_FinalRenderShader->SetFloat("Fog.Near", 1.f);
					m_FinalRenderShader->SetFloat("Fog.Far", 5000.f);
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
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		});
}

void SceneRenderer::ForwardPass()
{
	Renderer::Submit([this]()
		{	//draw
			Renderer::BeginRenderPass(m_FinalRenderPass);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glDisable(GL_BLEND);

			m_ForwardRenderShader->Use();
			m_ForwardRenderShader->SetMat4("Matrix.View", m_View);
			m_ForwardRenderShader->SetMat4("Matrix.Projection", m_Projection);

			//light set up
			{
				m_ForwardRenderShader->SetInt("LightNumbers", m_ActiveLights.size());
				int index = 0;
				for (auto& [lightPosition, lightDir, light] : m_ActiveLights)
				{
					std::string lightIndex = std::format("Light[{}].", index++);
					m_ForwardRenderShader->SetInt(lightIndex + "LightType", static_cast<int>(light.m_LightType));
					m_ForwardRenderShader->SetFloat3(lightIndex + "Position", lightPosition);
					m_ForwardRenderShader->SetFloat3(lightIndex + "Direction", lightDir);
					m_ForwardRenderShader->SetFloat(lightIndex + "InnerAngle", light.m_Angle.inner);
					m_ForwardRenderShader->SetFloat(lightIndex + "OuterAngle", light.m_Angle.outer);
					m_ForwardRenderShader->SetFloat(lightIndex + "FallOff", light.falloff);
					m_ForwardRenderShader->SetFloat3(lightIndex + "Ambient", light.Ambient);
					m_ForwardRenderShader->SetFloat3(lightIndex + "Diffuse", light.Diffuse);
					m_ForwardRenderShader->SetFloat3(lightIndex + "Specular", light.Specular);
				}
			}

			//hard coded for now
			{
				m_ForwardRenderShader->SetFloat3("globalAmbient", {});

				m_ForwardRenderShader->SetFloat("Attenuation.c1", 1.f);
				m_ForwardRenderShader->SetFloat("Attenuation.c2", 0.22f);
				m_ForwardRenderShader->SetFloat("Attenuation.c3", 0.20f);

				m_ForwardRenderShader->SetFloat3("Fog.Color", { 0.5f,0.5f,0.5f });
				m_ForwardRenderShader->SetFloat("Fog.Near", 1.f);
				m_ForwardRenderShader->SetFloat("Fog.Far", 5000.f);
			}

			m_ForwardRenderShader->SetFloat3("CameraPosition", m_CameraPosition);


			//todo: 
			for (auto& toDraw : m_DrawList)
			{
				toDraw.Mesh->GetMeshVBO()->BindToVertexArray();
				toDraw.Mesh->GetMeshEBO()->BindToVertexArray();
				m_DefaultMaterial->Bind();

				m_ForwardRenderShader->SetMat4("Matrix.Model", toDraw.Transform);
				m_ForwardRenderShader->SetMat4("Matrix.Normal", glm::transpose(glm::inverse(toDraw.Transform)));


				glDrawElements(GL_TRIANGLES, toDraw.Mesh->GetNumOfIndices(), GL_UNSIGNED_INT, nullptr);
			}

			Renderer::EndRenderPass();
		});
}

void SceneRenderer::BloomPass()
{
	Renderer::Submit([this]()
		{
			glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
		});

	//extract brightness
	Renderer::Submit([this]()
		{
			Renderer::BeginRenderPass(m_BloomExtractRenderPass, true);
			m_BloomExtractShader->Use();
			//Bloom Threshold
			m_BloomExtractShader->SetFloat("Threshold", 1.f);
			m_BloomExtractShader->SetInt("ToExtract", 0);
			m_FinalRenderPass->GetSpecification().TargetFramebuffer->GetColorTexture(0)->BindTexture(0);

			Renderer::DrawFullScreenQuad();
			Renderer::EndRenderPass();
		});

	//render blur 
	Renderer::Submit([this]()
		{
			bool horizontal = true, first_iteration = true;
			int amount = 10;
			m_BloomBlurShader->Use();
			m_BloomBlurRenderPass[0]->GetSpecification().TargetFramebuffer->GetColorTexture(0)->BindTexture(0);
			m_BloomBlurRenderPass[1]->GetSpecification().TargetFramebuffer->GetColorTexture(0)->BindTexture(1);
			m_BloomExtractRenderPass->GetSpecification().TargetFramebuffer->GetColorTexture(0)->BindTexture(2);

			for (unsigned int i = 0; i < amount; i++)
			{
				Renderer::BeginRenderPass(m_BloomBlurRenderPass[static_cast<int>(horizontal)], false);
				m_BloomBlurShader->SetInt("horizontal", static_cast<int>(horizontal));
				if (first_iteration)
				{
					m_BloomBlurShader->SetInt("image", 2);
					first_iteration = false;
				}
				else
				{
					m_BloomBlurShader->SetInt("image", static_cast<int>(!horizontal));
				}

				Renderer::DrawFullScreenQuad();
				horizontal = !horizontal;
				Renderer::EndRenderPass();
			}
		});


	//render bloom
	Renderer::Submit([this]()
		{
			Renderer::BeginRenderPass(m_FinalRenderPass, false);
			m_BloomRenderShader->Use();

			m_BloomRenderShader->SetInt("scene", 0);
			m_BloomRenderShader->SetInt("bloomBlur", 1);
			m_FinalRenderPass->GetSpecification().TargetFramebuffer->GetColorTexture(0)->BindTexture(0);
			m_BloomBlurRenderPass[1]->GetSpecification().TargetFramebuffer->GetColorTexture(0)->BindTexture(1);
			Renderer::DrawFullScreenQuad();
			Renderer::EndRenderPass();
		});


	Renderer::Submit([this]()
		{
			glDepthMask(GL_TRUE);
			glEnable(GL_DEPTH_TEST);
		});
}

void SceneRenderer::HDRPass()
{
	Renderer::Submit([this]()
		{
			std::shared_ptr<Texture> HDRColor = Texture::CopyTexture(m_FinalRenderPass->GetSpecification().TargetFramebuffer->GetColorTexture(0));
			Renderer::BeginRenderPass(m_FinalRenderPass, false);
			glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);

			m_HDRRenderShader->Use();
			m_HDRRenderShader->SetInt("HDRTexture", 0);
			HDRColor->BindTexture(0);
			Renderer::DrawFullScreenQuad();

			glDepthMask(GL_TRUE);
			glEnable(GL_DEPTH_TEST);
			Renderer::EndRenderPass();
		});
}

void SceneRenderer::DebugRenderingPass()
{
	//debug
	Renderer::Submit([this]()
		{	//draw
			Renderer::BeginRenderPass(m_FinalRenderPass, false);
			m_NormalRenderShader->Use();
			m_NormalRenderShader->SetMat4("View", m_View);
			m_NormalRenderShader->SetMat4("Projection", m_Projection);
			for (auto& toDraw : m_GeometryDrawList)
			{
				toDraw.Mesh->GetMeshVBO()->BindToVertexArray();
				toDraw.Mesh->GetMeshEBO()->BindToVertexArray();

				m_NormalRenderShader->SetMat4("Model", toDraw.Transform);
				glDrawElements(GL_TRIANGLES, toDraw.Mesh->GetNumOfIndices(), GL_UNSIGNED_INT, nullptr);
			}

			for (auto& toDraw : m_DrawList)
			{
				toDraw.Mesh->GetMeshVBO()->BindToVertexArray();
				toDraw.Mesh->GetMeshEBO()->BindToVertexArray();

				m_NormalRenderShader->SetMat4("Model", toDraw.Transform);
				glDrawElements(GL_TRIANGLES, toDraw.Mesh->GetNumOfIndices(), GL_UNSIGNED_INT, nullptr);
			}

			Renderer::EndRenderPass();
		});
}