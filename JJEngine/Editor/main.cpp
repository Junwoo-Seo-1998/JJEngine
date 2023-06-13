#include "Core/Application.h"
#include "Core/EntryPoint.h"
#include <Core/SceneManager.h>
#include "EditorLayer.h"
#include "TestScene.h"
#include "Core/Layer/LayerManager.h"
#include "Core/Utils/Log.h"
#include "Core/Utils/Math/MatrixMath.h"

class EditorApp : public Application
{
	virtual void UserDefinedInit() override
	{
		/*auto test1 = std::make_shared<TestScene>("Test");
		GetSceneManager()->enrollScene(0, test1);
		GetSceneManager()->setNextScene(0);*/
		EngineLog::Info(glm::vec2{ 1.f });
		EngineLog::Info(glm::vec3{ 1.f });
		EngineLog::Info(glm::vec4{1.f});
		EngineLog::Info(glm::mat3{ 1.f });
		EngineLog::Info(MatrixMath::BuildPerspectiveProjectionMatrixFovy(glm::radians(45.f), 1.f, 0.1f, 1000.f));
		GetLayerManager()->PushLayer(std::make_shared<EditorLayer>());
	}
};

std::shared_ptr<Application> CoreMain()
{
	return std::make_shared<EditorApp>();
}
