#pragma once
#include "Core/Layer/Layer.h"
#include <entt/entt.hpp>
#include <memory>
class Scene;
class IngameCameraControllerLayer : public Layer
{
	static constexpr unsigned int MAX_SHADOW_BUFFER = 6;
public:
	virtual ~IngameCameraControllerLayer() override;
	void OnAttach() override;
	void OnDetach() override;
	void OnStart() override;
	void OnUpdate() override;


private:
	std::shared_ptr<Scene> active_scene;
	bool camMove = false;

};

