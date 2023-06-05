/* Start Header -------------------------------------------------------
Author: Junwoo Seo
End Header --------------------------------------------------------*/
#pragma once
#include <memory>
#include "InputKeyCodes.h"

class Window;
class Input;
namespace InputStatics
{
	std::shared_ptr<Input> createInstance();
}

class Input
{
	friend Window;
public:
	Input() = delete;

	static bool IsPressed(Key key);
	static bool IsPressed(Mouse button);
	static bool IsTriggered(Key key);
	static bool IsTriggered(Mouse button);
	static bool IsRepeating(Key key);
	//window doesn't support repeating event for mouse
	//static bool IsRepeating(Mouse button);
	static bool IsReleased(Key key);
	static bool IsReleased(Mouse button);

	static bool IsMouseScrolled();

	static void SetMouseCursorEnable(bool input);

	static std::tuple<float, float> GetMousePosition();
	static std::tuple<float, float> GetLastMousePosition();

	static std::tuple<float, float> GetMouseOffset();
	static float GetMouseScrollOffset();
private:
	static void Reset();
	static void PostEventUpdate();
	static void SetKey(Key key, bool state);
	static void SetMouseButton(Mouse button, bool state);
	static void SetMousePosition(float x, float y);
	static void SetMouseScroll(float yOffset);
private:

};