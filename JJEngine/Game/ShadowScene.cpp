#include "ShadowScene.h"
#include "IngameRendererLayer.h"
#include "Core/Application.h"
#include "Core/Entity/Entity.hpp"
#include "Core/Component/CameraComponent.h"
#include "Core/Component/LightComponent.h"
#include "Core/Component/MaterialComponent.h"

#include <imgui.h>
#include <glad.h>
void ShadowScene::Start()
{
	

	Entity camera = CreateEntity("Cam");
	TransformComponent& cameraTransform = camera.GetComponent<TransformComponent>();
	cameraTransform.Position = { 0.f, 0.f, -3.f };
	cameraTransform.LookAtDir({ 0.f, 0.f, 1.f });
	camera.AddComponent<CameraComponent>(CameraComponent{});



	Entity light1 = CreateEntity("Light 1");
	TransformComponent& lightTransform1 = light1.GetComponent<TransformComponent>();
	lightTransform1.Position = { 10.f, 0.f, 0.f };
	light1.AddComponent<LightComponent>(LightComponent{});

	Entity light2 = CreateEntity("Light 2");
	TransformComponent& lightTransform2 = light2.GetComponent<TransformComponent>();
	lightTransform2.Position = { 0.f, 0.f, 10.f };
	light2.AddComponent<LightComponent>(LightComponent{});

	std::shared_ptr<Shader> forwardShader = Shader::CreateShaderFromFile({
{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/test.vert"}},
{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/test.frag"} }
		});

	std::shared_ptr<Shader> shadowShader = Shader::CreateShaderFromFile({
{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/shadowSampling.vert"}},
{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/shadowSampling.frag"} }
		});

	MaterialComponent forwardShadowMaterial{};
	forwardShadowMaterial.forwardShader = forwardShader;
	forwardShadowMaterial.shadowSamplingShader = shadowShader;
	forwardShadowMaterial.isShadowed = true;
	forwardShadowMaterial.type = MaterialType::Forward;

	Entity object = CreateEntity("Object");
	object.AddComponent<MaterialComponent>(forwardShadowMaterial);
	Model objectModel;
	objectModel.GetMeshes().push_back(std::make_shared<Mesh>(Mesh::CreateSphere(20, 20, 1.f, { 0.f, 0.f, 0.f })));
	object.AddComponent<Model>(objectModel);

	Entity floor = CreateEntity("Floor");
	floor.AddComponent<MaterialComponent>(forwardShadowMaterial);
	TransformComponent& floorTransform = floor.GetComponent<TransformComponent>();
	floorTransform.Position = { 0.f, -3.f, 0.f };	
	floorTransform.Scale = { 10.f, 10.f, 10.f };
	Model floorModel;
	floorModel.GetMeshes().push_back(std::make_shared<Mesh>(Mesh::CreatePlane(10, 10)));
	floor.AddComponent<Model>(floorModel);



//	Model model;
//	model.GetMeshes().push_back(std::make_shared<Mesh>(Mesh::CreateSphere(20, 20, 1.f, { 0.f, 0.f, 0.f })));
//	
//	Model planeModel;
//	planeModel.GetMeshes().push_back(std::make_shared<Mesh>(Mesh::CreatePlane(20, 20)));
//
//	auto target = model.GetMeshes()[0];
//	auto plane = planeModel.GetMeshes()[0];
//	shadowSceneVAO = VertexArray::CreateVertexArray();
//	shadowSceneSampleFBO.push_back(FrameBuffer::CreateFrameBuffer({ 512, 512, { FrameBufferFormat::Depth } }));
//	shadowSceneSampleFBO.push_back(FrameBuffer::CreateFrameBuffer({ 512, 512, { FrameBufferFormat::Depth } }));
//	shadowSceneSampleFBO.push_back(FrameBuffer::CreateFrameBuffer({ 512, 512, { FrameBufferFormat::Depth } }));
//
//	
//	shadowSceneVAO->Bind();
//	target->GetMeshVBO() = VertexBuffer::CreateVertexBuffer(target->GetNumOfVertices() * sizeof(Vertex));
//	target->GetMeshEBO() = IndexBuffer::CreateIndexBuffer(target->GetNumOfIndices() * sizeof(int));
//	plane->GetMeshVBO() = VertexBuffer::CreateVertexBuffer(plane->GetNumOfVertices() * sizeof(Vertex));
//	plane->GetMeshEBO() = IndexBuffer::CreateIndexBuffer(plane->GetNumOfIndices() * sizeof(int));
//	shadowSceneVAO->UnBind();
//	AddObject(model);
//	AddObject(planeModel);
//
//
//

//
//	auto window = Application::Instance().GetWindow();
//
//	freeCam = FreeCamera(window->GetGLFWWindow(), get<0>(window->GetWidthAndHeight()), get<1>(window->GetWidthAndHeight()));
//	freeCam.SetCamSpeed(0.15f);
//	freeCam.SetPosition({ 0.f, 0.f, 5.f });
//	freeCam.nearPlane = 0.1f;
//	freeCam.farPlane = 100.f;
//	light1 = FreeCamera(window->GetGLFWWindow(), get<0>(window->GetWidthAndHeight()), get<1>(window->GetWidthAndHeight()));
//	light1.camFront = { -1.f, 0.f, 0.f };
//	light1.SetPosition({ 3.f, 0.f, 0.f });
//	light1.nearPlane = 0.1f;
//	light1.farPlane = 100.f;
//	light2 = FreeCamera(window->GetGLFWWindow(), get<0>(window->GetWidthAndHeight()), get<1>(window->GetWidthAndHeight()));
//	light2.camFront = { 0.f, 0.f, -1.f };
//	light2.SetPosition({ 0.f, 0.f, 3.f });
//	light2.nearPlane = 0.1f;
//	light2.farPlane = 100.f;
//	freeCam.ComputeViewProjMats();
//	light1.ComputeViewProjMats();
//	light2.ComputeViewProjMats();
//	controller = &freeCam;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void ShadowScene::Update()
{
	/*controller->Update();
	controller->ComputeViewProjMats();

	VariableContainerType shadowSamplingVariables;
	shadowSamplingVariables.emplace("Model", objectGroups[0]);
	shadowSamplingVariables.emplace("Shader", shadowShader);
	shadowSamplingVariables.emplace("ShadowSaples", shadowSceneSampleFBO);
	glm::ivec2 shadowResolution{ 512, 512 };
	glm::ivec2 polygonOffset{ 1, 1 };
	shadowSamplingVariables.emplace("Shadow Resolution", shadowResolution);
	shadowSamplingVariables.emplace("Polygon Offset", polygonOffset);
	shadowSamplingVariables.emplace("toWorld", glm::scale(glm::mat4(1.0f), { 1.f, 1.f, 1.f }));
	shadowSamplingVariables.emplace("toLights", std::vector<glm::mat4>{light1.vpMat, light2.vpMat});

	RenderingLayer::GetInstance()->AddRenderCommand(CommandType::ShadowSampling, shadowSamplingVariables);


	VariableContainerType drawVariables;
	drawVariables.emplace("Model", objectGroups[0]);
	drawVariables.emplace("Shader", shader);
	drawVariables.emplace("VAO", shadowSceneVAO);
	drawVariables.emplace("toWorld", glm::scale(glm::mat4(1.0f), { 1.f, 1.f, 1.f }));
	drawVariables.emplace("toView", controller->vpMat);
	drawVariables.emplace("lights", std::vector<glm::vec3>{light1.cam, light2.cam});
	drawVariables.emplace("toLights", std::vector<glm::mat4>{light1.vpMat, light2.vpMat});

	drawVariables.emplace("ShadowSaples", shadowSceneSampleFBO);

	RenderingLayer::GetInstance()->AddRenderCommand(CommandType::Draw, drawVariables);

	VariableContainerType drawVariables2;
	drawVariables2.emplace("Model", objectGroups[1]);
	drawVariables2.emplace("Shader", shader);
	drawVariables2.emplace("VAO", shadowSceneVAO);
	drawVariables2.emplace("toWorld", glm::translate(glm::mat4(1.0f), { 0.f, -5.f, 0.f }) * glm::scale(glm::mat4(1.0f), { 20.f, 20.f, 20.f }));
	drawVariables2.emplace("toView", controller->vpMat);
	drawVariables2.emplace("lights", std::vector<glm::vec3>{light1.cam, light2.cam});
	drawVariables2.emplace("toLights", std::vector<glm::mat4>{light1.vpMat, light2.vpMat});

	drawVariables2.emplace("ShadowSaples", shadowSceneSampleFBO);
	RenderingLayer::GetInstance()->AddRenderCommand(CommandType::Draw, drawVariables2);

	ImGui::Begin("viewport");
	ImGui::Image((void*)shadowSceneSampleFBO[0]->GetDepthTexture()->GetTextureID(), { 200, 200 }, { 0,1 }, { 1,0 });
	ImGui::SameLine();
	ImGui::Image((void*)shadowSceneSampleFBO[1]->GetDepthTexture()->GetTextureID(), { 200, 200 }, { 0,1 }, { 1,0 });
	ImGui::End();

	ImGui::Begin("control");
	if (ImGui::Button("light1", { 50, 20 }))
	{
		controller = &light1;

	}
	ImGui::SameLine();

	if (ImGui::Button("light2", { 50, 20 }))
	{
		controller = &light2;

	}
	ImGui::SameLine();

	if (ImGui::Button("Cam", { 50, 20 }))
	{
		controller = &freeCam;
	}

	ImGui::End();*/
}

void ShadowScene::PostUpdate()
{
	
}
