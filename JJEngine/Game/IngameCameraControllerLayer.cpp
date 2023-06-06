#include "IngameCameraControllerLayer.h"
#include "Core/Input/Input.h"
#include "Core/Scene.h"
#include "Core/SceneManager.h"
#include "Core/Entity/Entity.hpp"
#include "Core/Component/CameraComponent.h"
#include "Core/Component/TransformComponent.h"
#include "ShadowScene.h"


void MouseCamControl(TransformComponent& camT, double xpos, double ypos);

IngameCameraControllerLayer::~IngameCameraControllerLayer()
{
}

void IngameCameraControllerLayer::OnAttach()
{
	sc = std::make_shared<ShadowScene>();
	active_scene = sc;
	active_scene->Start();
}

void IngameCameraControllerLayer::OnDetach()
{
}

void IngameCameraControllerLayer::OnStart()
{
}

void IngameCameraControllerLayer::OnUpdate()
{
	if (Input::IsReleased(KeyCode::Space)) {
		camMove = !camMove;
	}
	if (camMove) {
		Input::SetMouseCursorEnable(false);
	}
	else {
		Input::SetMouseCursorEnable(true);
	}

	auto& reg = active_scene->GetRegistry();
	auto camIDs = reg.view<CameraComponent>();

	for (auto camID : camIDs)
	{
		Entity e{ camID, active_scene.get()};
		auto& camera = e.GetComponent<CameraComponent>();
		auto& cameraTransform = e.GetComponent<TransformComponent>();		
		if (e.GetComponent<CameraComponent>().main_cam)
		{
			if (Input::IsPressed(KeyCode::W)) {
				cameraTransform.Position += camera.cam_speed * cameraTransform.GetForward();
			}
			if (Input::IsPressed(KeyCode::S)) {
				cameraTransform.Position -= camera.cam_speed * cameraTransform.GetForward();
			}
			if (Input::IsPressed(KeyCode::A)) {
				cameraTransform.Position -= camera.cam_speed * cameraTransform.GetRight();
			}
			if (Input::IsPressed(KeyCode::D)) {
				cameraTransform.Position += camera.cam_speed * cameraTransform.GetRight();
			}
			if (Input::IsPressed(KeyCode::Q)) {
				cameraTransform.Position.y += camera.cam_speed;
			}
			if (Input::IsPressed(KeyCode::E)) {
				cameraTransform.Position.y -= camera.cam_speed;
			}
			if (camMove)
			{
				MouseCamControl(cameraTransform, get<0>(Input::GetMousePosition()), get<1>(Input::GetMousePosition()));
			}
		}
	}
	
}

void MouseCamControl(TransformComponent& camT, double xpos, double ypos)
{
	double xoffset = get<0>(Input::GetMouseOffset());
	double yoffset = get<1>(Input::GetMouseOffset());

	float sensitivity = 0.0005f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camT.Rotation.y += static_cast<float>(xoffset);
	camT.Rotation.x += static_cast<float>(yoffset);

	//if (camT.Rotation.x > glm::radians(89.0f))
	//	camT.Rotation.x = glm::radians(89.0f);
	//if (camT.Rotation.x < glm::radians(-89.0f))
	//	camT.Rotation.x = glm::radians(-89.0f);
}
