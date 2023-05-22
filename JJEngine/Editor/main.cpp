#include "Core/EntryPoint.h"
#include <Core/SceneManager.h>
#include "TestScene.h"

class EditorApp : public Application
{
	TestScene test1{ "editor" };
	virtual void UserDefinedInit() override
	{
		GetSceneManager()->enrollScene(0, test1);
		GetSceneManager()->setNextScene(0);
	}
};

std::shared_ptr<Application> CoreMain()
{
	return std::make_shared<EditorApp>();
}
