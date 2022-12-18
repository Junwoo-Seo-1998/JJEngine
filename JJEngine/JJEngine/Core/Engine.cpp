/* Start Header -------------------------------------------------------
Project: JJEngine 
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/16/2022
End Header-------------------------------------------------------- */

#include "Engine.h"
#include "Window.h"
#include "Input/Input.h"

namespace statics
{
	std::shared_ptr<JJEngine> createInstance() {
		return std::shared_ptr<JJEngine>(new JJEngine{});
	}
	std::shared_ptr<JJEngine> instance = createInstance();
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

		window->update([&]()
		{
			if(Input::IsKeyPressed(KeyCode::A))
			{
				std::cout << "test" << std::endl;
			}
		});

	} while (engineLoop);
}
