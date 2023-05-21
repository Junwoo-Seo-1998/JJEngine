#include "ShadowScene.h"
#include "Core/Graphics/Graphics.h"
#include <glad.h>
void ShadowScene::Load()
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

	freeCam.SetCamSpeed(0.15f);
	freeCam.SetPosition({ 0.f, 0.f, 20.f });
	freeCam.nearPlane = 0.1f;
	freeCam.farPlane = 100.f;

	shader = Shader::CreateShaderFromFile({
{ ShaderType::VertexShader,{"Resources/Shaders/version.glsl","Resources/Shaders/simple.vert"}},
{ ShaderType::FragmentShader,{"Resources/Shaders/version.glsl","Resources/Shaders/simple.frag"} }
		});
}

void ShadowScene::Update(double dt)
{
	glClearColor(0.3, 0.7, 0.3, 1.0);




}

void ShadowScene::Draw()
{
	VariableContainerType variables;
	variables.emplace("Model", objectGroups[0]);
	variables.emplace("Shader", shader);
	variables.emplace("VAO", shadowSceneVAO);

	Graphics::GetInstance()->AddRenderCommand(CommandType::Draw, variables);

	Graphics::GetInstance()->ExecuteRenderCommands();
}
