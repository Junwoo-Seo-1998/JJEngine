#include "Core/test.h"
#include "Core/Engine.h"
#include "Core/SceneManager.h"
#include "TestScene.h"
#include "Core/Utils/Log.h"
//#include "Core/ComponentManager.h"

int main(void)
{
	EngineLog::Info("Engine Init!");
	JJEngine::init();
	TestScene test1{" test 1 "};
	TestScene test2{" test 2 "};
	
	JJEngine::GetSceneManager()->enrollScene(0, test1);
	JJEngine::GetSceneManager()->enrollScene(1, test2);
	JJEngine::GetSceneManager()->setNextScene(1);
	Log::Info("hello! from game");
	JJEngine::update();
	JJEngine::shutdown();
	hello();
	return 0;
}
