/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs350_1
Author: Junwoo Seo, junwoo.seo, 0055213
End Header --------------------------------------------------------*/
#include "Input.h"
#include "Core/Engine.h"
#include "Core/Window.h"
#include <GLFW/glfw3.h>
#include <array>
#include <stack>

#include "Core/Utils/Log.h"

namespace InputStatics
{
	std::array<bool, static_cast<size_t>(Key::Last)>  s_pressed = {};/*!< Array of keys pressed this frame*/
	std::array<bool, static_cast<size_t>(Key::Last)>  s_released = {};/*!< Array of keys released this frame*/
	std::array<bool, static_cast<size_t>(Key::Last)>  s_triggered = {};/*!< Array of keys triggered this frame*/
	std::array<bool, static_cast<size_t>(Key::Last)>  s_repeating = {};;/*!< Array of keys repeated this frame*/

	std::array<bool, static_cast<size_t>(Mouse::Last)> s_mouse_pressed = {};/*!< Array of keys pressed this frame*/
	std::array<bool, static_cast<size_t>(Mouse::Last)>  s_mouse_released = {};/*!< Array of keys released this frame*/
	std::array<bool, static_cast<size_t>(Mouse::Last)>  s_mouse_triggered = {};/*!< Array of keys triggered this frame*/
	std::array<bool, static_cast<size_t>(Mouse::Last)>  s_mouse_repeating = {};;/*!< Array of keys repeated this frame*/

	std::stack<Key> s_unpressed;            /*!< The array of keys to unpress*/
	std::stack<Mouse> s_mouse_unpressed;            /*!< The array of keys to unpress*/

	std::tuple<float, float> s_LastMousePos;
	std::tuple<float, float> s_MousePos;
	std::tuple<float, float> s_MouseOffset;
}

bool Input::IsPressed(Key key)
{
	return InputStatics::s_pressed[static_cast<unsigned short>(key)];
}

bool Input::IsPressed(Mouse button)
{
	return InputStatics::s_mouse_pressed[static_cast<unsigned short>(button)];
}

bool Input::IsTriggered(Key key)
{
	return InputStatics::s_triggered[static_cast<unsigned short>(key)];
}

bool Input::IsTriggered(Mouse button)
{
	return InputStatics::s_mouse_triggered[static_cast<unsigned short>(button)];
}

bool Input::IsRepeating(Key key)
{
	return InputStatics::s_repeating[static_cast<unsigned short>(key)];
}

/*bool Input::IsRepeating(Mouse button)
{
	return InputStatics::s_mouse_repeating[static_cast<unsigned short>(button)];
}*/

bool Input::IsReleased(Key key)
{
	return InputStatics::s_released[static_cast<unsigned short>(key)];
}

bool Input::IsReleased(Mouse button)
{
	return InputStatics::s_mouse_released[static_cast<unsigned short>(button)];
}

void Input::SetMouseCursorEnable(bool input)
{
	glfwSetInputMode(JJEngine::GetWindow()->GetGLFWWindow(), GLFW_CURSOR, input ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

std::tuple<float, float> Input::GetMousePosition()
{
	return InputStatics::s_MousePos;
}

std::tuple<float, float> Input::GetLastMousePosition()
{
	return InputStatics::s_LastMousePos;
}

std::tuple<float, float> Input::GetMouseOffset()
{
	return InputStatics::s_MouseOffset;
}

void Input::Reset()
{
	/*Only reset the pressed values that are not being pressed*/
	while (!InputStatics::s_unpressed.empty())
	{
		InputStatics::s_pressed[static_cast<unsigned short>(InputStatics::s_unpressed.top())] = false;/*Reset the pressed value*/
		InputStatics::s_unpressed.pop();
	}

	while (!InputStatics::s_mouse_unpressed.empty())
	{
		InputStatics::s_mouse_pressed[static_cast<unsigned short>(InputStatics::s_mouse_unpressed.top())] = false;/*Reset the pressed value*/
		InputStatics::s_mouse_unpressed.pop();
	}

	InputStatics::s_triggered.fill(false);
	InputStatics::s_repeating.fill(false);
	InputStatics::s_released.fill(false);

	InputStatics::s_mouse_triggered.fill(false);
	InputStatics::s_mouse_repeating.fill(false);
	InputStatics::s_mouse_released.fill(false);

	InputStatics::s_MouseOffset = { 0.f,0.f };
}

void Input::PostEventUpdate()
{
	//place holder for later use
}

void Input::SetKey(Key key, bool state)
{
	const int key_index = static_cast<unsigned short>(key);
	if (state)
	{
		if (InputStatics::s_pressed[key_index])
		{
			InputStatics::s_repeating[key_index] = true;
		}
		else
		{
			InputStatics::s_pressed[key_index] = true;
			InputStatics::s_triggered[key_index] = true;
		}
	}
	else
	{
		InputStatics::s_unpressed.push(key);
		InputStatics::s_released[key_index] = true;
	}
}

void Input::SetMouseButton(Mouse button, bool state)
{
	const int button_index = static_cast<unsigned short>(button);
	if (state)
	{
		if(InputStatics::s_mouse_pressed[button_index])
		{
			InputStatics::s_mouse_repeating[button_index] = true;
		}
		else
		{
			InputStatics::s_mouse_pressed[button_index] = true;
			InputStatics::s_mouse_triggered[button_index] = true;
		}
	}
	else
	{
		InputStatics::s_mouse_unpressed.push(button);
		InputStatics::s_mouse_released[button_index] = true;
	}
}

void Input::SetMousePosition(float x, float y)
{
	auto [last_x, last_y] = InputStatics::s_LastMousePos;
	InputStatics::s_MouseOffset = { x - last_x, last_y - y };
	InputStatics::s_MousePos = { x,y };
	InputStatics::s_LastMousePos = InputStatics::s_MousePos;
}

