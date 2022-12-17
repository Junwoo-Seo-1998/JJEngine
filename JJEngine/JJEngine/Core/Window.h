/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/16/2022
End Header-------------------------------------------------------- */
#pragma once
struct GLFWwindow;
class Window
{
	GLFWwindow* window{};
	int windowWidth{ 1200 }, windowHeight{ 800 };
public:
	Window();
	~Window();
	void update();
	bool shouldClose();

};
