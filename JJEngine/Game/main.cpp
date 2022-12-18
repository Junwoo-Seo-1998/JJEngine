#include "Core/test.h"
#include "Core/Engine.h"
#include "Core/SceneManager.h"
#include "TestScene.h"
int main(void)
{
	TestScene test1{" test 1 "};
	TestScene test2{" test 2 "};

	JJEngine::EngineOn();
	JJEngine::GetSceneManager()->enrollScene(0, test1);
	JJEngine::GetSceneManager()->enrollScene(1, test2);
	JJEngine::GetSceneManager()->setNextScene(1);
	JJEngine::update();
	JJEngine::EngineOff();
	hello();
	return 0;
}