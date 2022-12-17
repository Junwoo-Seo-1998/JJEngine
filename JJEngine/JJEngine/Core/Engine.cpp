/* Start Header -------------------------------------------------------
Project: JJEngine 
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/16/2022
End Header-------------------------------------------------------- */

#include "Engine.h"
#include "Window.h"
namespace statics
{
	std::shared_ptr<JJEngine> instance = nullptr;
}
JJEngine::JJEngine()
{
	window = new Window{};
}

JJEngine::~JJEngine()
{
	delete window;
}

std::shared_ptr<JJEngine> JJEngine::instance()
{
	if (statics::instance == nullptr) statics::instance = std::make_shared<JJEngine>(*(new JJEngine{}));
	return statics::instance;
}

void JJEngine::update()
{
	bool engineLoop{ true };

	do
	{
		if (window->shouldClose() == true) {
			engineLoop = false;
		}

		window->update();

	} while (engineLoop);
}
