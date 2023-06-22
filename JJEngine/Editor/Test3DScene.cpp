#include "Test3DScene.h"
#include "Core/Application.h"
#include "Core/Entity/Entity.hpp"
#include "Core/Component/CameraComponent.h"
#include "Core/Component/LightComponent.h"
#include "Core/Component/MaterialComponent.h"

#include <imgui.h>
#include <glad.h>
void Test3DScene::Start()
{




	Entity light1 = CreateEntity("Light 1");
	TransformComponent& lightTransform1 = light1.GetComponent<TransformComponent>();
	lightTransform1.Position = { 3.f, 2.f, 0.f };
	light1.AddComponent<LightComponent>(LightComponent{});

	Entity light2 = CreateEntity("Light 2");
	TransformComponent& lightTransform2 = light2.GetComponent<TransformComponent>();
	lightTransform2.Position = { 0.f, 2.f, 3.f };
	light2.AddComponent<LightComponent>(LightComponent{});

	std::shared_ptr<Shader> forwardShader = Shader::CreateShaderFromFile({
{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/ForwardRendering.vert"}},
{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/ForwardRendering.frag"} }
		});


	std::shared_ptr<Shader> deferredShader = Shader::CreateShaderFromFile({
{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/DeferredRendering.vert"}},
{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/DeferredRendering.frag"} }
		});


	std::shared_ptr<Shader> shadowShader = Shader::CreateShaderFromFile({
{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/shadowSampling.vert"}},
{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/shadowSampling.frag"} }
		});

	std::shared_ptr<Shader> gBufferShader = Shader::CreateShaderFromFile({
{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/GBufferSampling.vert"}},
{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/GBufferSampling.frag"} }
		});


	MaterialComponent deferredShadowMaterial{};
	deferredShadowMaterial.shadowSamplingShader = shadowShader;
	deferredShadowMaterial.defferedFirstPassShader = gBufferShader;
	deferredShadowMaterial.defferedSecondPassShader = deferredShader;
	deferredShadowMaterial.isShadowed = true;
	deferredShadowMaterial.type = MaterialType::Deferred;


	MaterialComponent forwardShadowMaterial{};
	forwardShadowMaterial.forwardShader = forwardShader;
	forwardShadowMaterial.shadowSamplingShader = shadowShader;
	forwardShadowMaterial.isShadowed = true;
	forwardShadowMaterial.type = MaterialType::Forward;

	Entity object = CreateEntity("Object");
	object.AddComponent<MaterialComponent>(deferredShadowMaterial);
	Model objectModel;
	objectModel.GetMeshes().push_back(std::make_shared<Mesh>(Mesh::CreateSphere(20, 20, 1.f, { 0.f, 0.f, 0.f })));
	object.AddComponent<Model>(objectModel);


	Entity object2 = CreateEntity("Object2");
	TransformComponent& object2Transform = object2.GetComponent<TransformComponent>();
	object2Transform.Position = {6.f, 1.f, 8.f};
	object2Transform.Scale = { 1.f, 2.f, 1.f };
	object2.AddComponent<MaterialComponent>(deferredShadowMaterial);
	Model object2Model;
	object2Model.GetMeshes().push_back(std::make_shared<Mesh>(Mesh::CreateSphere(20, 20, 1.f, { 0.f, 0.f, 0.f })));
	object2.AddComponent<Model>(objectModel);


	MaterialComponent forwardMaterial{};
	forwardMaterial.forwardShader = forwardShader;
	forwardMaterial.isShadowed = false;
	forwardMaterial.type = MaterialType::Forward;

	MaterialComponent DeferredMaterial{};
	DeferredMaterial.defferedFirstPassShader = gBufferShader;
	DeferredMaterial.defferedSecondPassShader = deferredShader;
	DeferredMaterial.isShadowed = false;
	DeferredMaterial.type = MaterialType::Deferred;

	Entity floor = CreateEntity("Floor");
	floor.AddComponent<MaterialComponent>(forwardMaterial);
	TransformComponent& floorTransform = floor.GetComponent<TransformComponent>();
	floorTransform.Position = { 0.f, -3.f, 0.f };
	floorTransform.Scale = { 100.f, 100.f, 100.f };
	Model floorModel;
	floorModel.GetMeshes().push_back(std::make_shared<Mesh>(Mesh::CreatePlane(10, 10)));
	floor.AddComponent<Model>(floorModel);

	Scene::Start();

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

void Test3DScene::Update()
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

void Test3DScene::PostUpdate()
{

}