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
	static bool IsRepeating(Key key);
	static bool IsReleased(Key key);
	static bool IsReleased(Mouse button);

	static void SetMouseCursorEnable(bool input);

	static std::tuple<float, float> GetMousePosition();
	static std::tuple<float, float> GetLastMousePosition();

	static std::tuple<float, float> GetMouseOffset();
private:
	static void Reset();
	static void SetKey(Key key, bool state);
	static void SetMouseButton(Mouse button, bool state);
	static void SetMousePosition(float x, float y);
private:

};