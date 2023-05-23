#include "Core/Application.h"
#include "Core/EntryPoint.h"
#include <Core/SceneManager.h>
#include "EditorLayer.h"
#include "TestScene.h"
#include "Core/Layer/LayerManager.h"

class EditorApp : public Application
{
	virtual void UserDefinedInit() override
	{
		auto test1 = std::make_shared<TestScene>("Test");
		GetSceneManager()->enrollScene(0, test1);
		GetSceneManager()->setNextScene(0);

		GetLayerManager()->PushLayer(std::make_shared<EditorLayer>());
	}
};

std::shared_ptr<Application> CoreMain()
{
	return std::make_shared<EditorApp>();
}
