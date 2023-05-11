#include <Core/Window.h>
#include <Core/SceneManager.h>
#include <Core/ImGui/ImGuiRenderer.h>
#include "TestScene.h"

int main(void)
{
	// Editor engine??

	Window* window = new Window{};
	SceneManager* sceneManager = new SceneManager{};
	ImGuiRenderer::Instance()->Init(window->GetGLFWWindow());
	TestScene test1{ " editor eeeeeeeeeeeeeeee " };
	TestScene test2{ " editor eeeeeeeeeeeeeeee 2" };
	sceneManager->enrollScene(0, test1);
	sceneManager->enrollScene(1, test2);
	sceneManager->setNextScene(1);
	bool engineLoop{ true };
	std::chrono::system_clock::time_point lastTick = std::chrono::system_clock::now();
	double dt{ };
	do
	{
		const std::chrono::time_point now{ std::chrono::system_clock::now() };
		dt = { std::chrono::duration<double>(now - lastTick).count() };
		if (window->shouldClose() == true) {
			sceneManager->exit();
			engineLoop = false;
		}

		window->update([&]()
			{
				ImGuiRenderer::Instance()->GuiBegin();
				sceneManager->update(dt);
				ImGuiRenderer::Instance()->GuiEnd();
			});

		lastTick = now;

	} while (engineLoop);


	ImGuiRenderer::Instance()->Shutdown();

	return 0;
}