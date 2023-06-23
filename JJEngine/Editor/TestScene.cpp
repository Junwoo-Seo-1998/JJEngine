#include "TestScene.h"
#include "Core/Graphics/Texture.h"
#include "Core/Utils/File.h"
#include "imgui.h"
#include <Core/Utils/Log.h>
#include "Core/Time.h"
#include "Core/Component/CameraComponent.h"
#include "Core/Entity/Entity.hpp"
#include "Core/Component/SpriteRendererComponent.h"
#include "Core/Component/RigidBody2DComponent.h"
#include "Core/Component/BoxCollider2DComponent.h"
#include "Core/Application.h"
#include "Core/Asset/Manager/AssetManager.h"
#include "Core/Component/LightComponent.h"
#include "Core/Component/MeshComponent.h"
#include "Core/Component/ScriptComponent.h"
#include "Core/Graphics/Mesh.h"
#include "Core/Graphics/MeshFactory.h"

TestScene::TestScene(std::string t)
	:Scene(t)
{

}

void TestScene::Start()
{

	auto cam=CreateEntity("Camera");
	auto& camComp=cam.AddComponent<CameraComponent>();
	camComp.IsMainCamera = true;
	cam.Transform().Position = { 0,1.52f,7.5f };

	auto ground = CreateEntity("Ground");
	auto& groundSprite = ground.AddComponent<SpriteRendererComponent>();
	groundSprite.color = { 0.5f,0.25f, 0.2f, 1.f };
	ground.Transform().Scale = { 5,0.25f,1 };
	ground.Transform().Position.y = -1.f;
	ground.AddComponent<BoxCollider2DComponent>();
	ground.AddComponent<RigidBody2DComponent>();

	auto ground2 = CreateEntity("Ground2");
	auto& groundSprite2 = ground2.AddComponent<SpriteRendererComponent>();
	groundSprite2.color = { 0.5f,0.25f, 0.2f, 1.f };
	ground2.Transform().Scale = { 1.f,0.25f,1 };
	ground2.Transform().Position.y = 1.21f;
	ground2.Transform().Position.x = 0.67f;
	ground2.Transform().Rotation.z = glm::radians(29.23f);
	ground2.AddComponent<BoxCollider2DComponent>();
	ground2.AddComponent<RigidBody2DComponent>();

	auto temp = CreateEntity("Test Texture Entity");

	temp.Transform().Position = { 0.f, 2.50f, 0 };
	auto& sprite = temp.AddComponent<SpriteRendererComponent>();
	sprite.asset = Application::Instance().GetAssetManager()->GetCastedAsset<Asset_Texture>(Application::Instance().GetAssetManager()->GetHandleFromPath("./Resources/Textures/test.jpg")); //Texture::CreateTexture(File::ReadImageToTexture("Resources/Textures/test.jpg"));

	/*temp.AddComponent<BoxCollider2DComponent>();
	auto& body = temp.AddComponent<RigidBody2DComponent>();
	body.Type = RigidBody2DComponent::BodyType::Dynamic;*/

	auto& script=temp.AddComponent<ScriptComponent>();
	script.Name = "Game.Player";

	auto light = CreateEntity("Test light");
	light.Transform().Position = { 2.f, 0.f,0.f };
	light.AddComponent<LightComponent>();


	auto ThreeDObject = CreateEntity("Test 3D Object");
	auto& meshcomp = ThreeDObject.AddComponent<MeshComponent>();
	meshcomp.mesh = MeshFactory::CreateBox(glm::vec3{1.f});

}

void TestScene::Update()
{
	float dt = Time::GetDelta();
	/*framebuffer->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//draw test
	shader->Use();
	vertex_array->Bind();
	shader->SetInt("testTexture", 0);
	test_texture->BindTexture();
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	framebuffer->UnBind();
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);*/
}
