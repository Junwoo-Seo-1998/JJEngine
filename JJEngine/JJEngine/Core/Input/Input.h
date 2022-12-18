/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/16/2022
End Header-------------------------------------------------------- */
#pragma once
#include <array>
#include <memory>
#include <stack>
#include "InputKeyCodes.h"

class Window;
class Input;
namespace InputStatics
{
	std::shared_ptr<Input> createInstance();
}

class Input
{
	friend std::shared_ptr<Input> InputStatics::createInstance();
	friend Window;
public:
	static std::shared_ptr<Input> instance();
	static bool IsKeyPressed(KeyCode key);
	static bool IsTriggered(KeyCode key);
	static bool IsRepeating(KeyCode key);
	static bool IsKeyReleased(KeyCode key);
private:
	Input() = default;
	static void Reset();
	static void SetKey(KeyCode key, bool state, bool repeating);

};
