#include "ShadowScene.h"
#include "Core/Graphics/RenderingLayer.h"
#include "Core/Application.h"

#include <glad.h>
void ShadowScene::Start()
{
	Model model;
	model.GetMeshes().push_back(std::make_shared<Mesh>(Mesh::CreateSphere(20, 20, 1.f, { 0.f, 0.f, 0.f })));
	auto target = model.GetMeshes()[0];
	shadowSceneVAO = VertexArray::CreateVertexArray();

	shadowSceneVAO->Bind();
	target->GetMeshVBO() = VertexBuffer::CreateVertexBuffer(target->GetNumOfVertices() * sizeof(Vertex));
	target->GetMeshEBO() = IndexBuffer::CreateIndexBuffer(target->GetNumOfIndices() * sizeof(int));
	shadowSceneVAO->UnBind();
	AddObject(model);



	shader = Shader::CreateShaderFromFile({
{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/test.vert"}},
{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/test.frag"} }
		});

//	shadowShader = Shader::CreateShaderFromFile({
//{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/shadowSampling.vert"}},
//{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/shadowSampling.frag"} }
//		});

	auto window = Application::Instance().GetWindow();

	freeCam = FreeCamera(window->GetGLFWWindow(), get<0>(window->GetWidthAndHeight()), get<1>(window->GetWidthAndHeight()));
	freeCam.SetCamSpeed(0.15f);
	freeCam.SetPosition({ 0.f, 0.f, 20.f });
	freeCam.nearPlane = 0.1f;
	freeCam.farPlane = 100.f;
	light1 = FreeCamera(window->GetGLFWWindow(), get<0>(window->GetWidthAndHeight()), get<1>(window->GetWidthAndHeight()));
	light1.SetCamSpeed(0.15f);
	light1.SetPosition({ 20.f, 0.f, 0.f });
	light1.nearPlane = 0.1f;
	light1.farPlane = 100.f;
	light2 = FreeCamera(window->GetGLFWWindow(), get<0>(window->GetWidthAndHeight()), get<1>(window->GetWidthAndHeight()));
	light2.SetCamSpeed(0.15f);
	light2.SetPosition({ 0.f, 20.f, 0.f });
	light2.nearPlane = 0.1f;
	light2.farPlane = 100.f;
}

void ShadowScene::Update()
{
	freeCam.Update();
	freeCam.ComputeViewProjMats();

	glClearColor(0.3, 0.7, 0.3, 1.0);
	//VariableContainerType shadowSamplingVariables;
	//shadowSamplingVariables.emplace("Model", objectGroups[0]);
	//shadowSamplingVariables.emplace("Shader", shadowShader);
	//shadowSamplingVariables.emplace("FBO", shadowSceneSampleFBO);
	//glm::ivec2 shadowResolution{ 512, 512 };
	//glm::ivec2 polygonOffset{ 1, 1 };
	//shadowSamplingVariables.emplace("Shadow Resolution", shadowResolution);
	//shadowSamplingVariables.emplace("Polygon Offset", polygonOffset);
	//shadowSamplingVariables.emplace("toLight", std::vector<glm::mat4>{light1.vpMat, light2.vpMat});

	//RenderingLayer::GetInstance()->AddRenderCommand(CommandType::ShadowSampling, shadowSamplingVariables);


	VariableContainerType drawVariables;
	drawVariables.emplace("Model", objectGroups[0]);
	drawVariables.emplace("Shader", shader);
	drawVariables.emplace("VAO", shadowSceneVAO);
	drawVariables.emplace("toView", freeCam.vpMat);

	drawVariables.emplace("lights", std::vector<glm::vec3>{light1.cam, light2.cam});

	RenderingLayer::GetInstance()->AddRenderCommand(CommandType::Draw, drawVariables);
}

void ShadowScene::PostUpdate()
{
	
}
