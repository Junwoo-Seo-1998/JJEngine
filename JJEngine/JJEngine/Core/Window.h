/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/16/2022
End Header-------------------------------------------------------- */
#pragma once
#include <functional>
struct GLFWwindow;
class Window
{
	GLFWwindow* window{};
	int windowWidth{ 1200 }, windowHeight{ 800 };
public:
	Window();
	~Window();
	void update(std::function<void()> updateCallback);
	bool shouldClose();

	GLFWwindow* GetGLFWWindow();
	std::tuple<int, int> GetWidthAndHeight();

private:
	void SetWindowSize(int width, int height);
	static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void GLFWMouseCallback(GLFWwindow* window, int button, int action, int mods);
	static void GLFWMousePositionCallback(GLFWwindow* window, double xposIn, double yposIn);
	static void GLFWResizeCallback(GLFWwindow* window, int width, int height);

};
