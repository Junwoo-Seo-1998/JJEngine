/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang
Platform: x64
Date: 12/16/2022
End Header-------------------------------------------------------- */
#include "Window.h"
#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Input/Input.h"

Window::Window()
{
	if (!glfwInit())
	{
		std::cout << "GLFW init Failed" << std::endl;
		glfwTerminate();

		return;
	}
	/*
	 *no need for glad
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	*/

	window = glfwCreateWindow(windowWidth, windowHeight, "JJEngine",
		nullptr, // which monitor (if full-screen mode)
		nullptr); // if sharing context with another window
	if (!window) {
		std::cout << "GLFW can't make window";
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	//glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	int err = gladLoadGL();
	if (err != 1) {
		std::cerr << "Unable to initialize Glad - error: " << " abort program" << std::endl;
		glfwTerminate();
		return;
	}
	//glfwSetWindowUserPointer(window, &m_WindowData);
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
		{
			//WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					Input::instance()->SetKey(static_cast<KeyCode>(key), true, false);
					break;
				}
				case GLFW_RELEASE:
				{
					Input::instance()->SetKey(static_cast<KeyCode>(key), false, false);
					break;
				}
				case GLFW_REPEAT:
				{
					Input::instance()->SetKey(static_cast<KeyCode>(key), true, true);
					break;
				}
			}
		});


	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

Window::~Window()
{
	glfwTerminate();
}

void Window::update(std::function<void()> updateCallback)
{
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
	Input::instance()->Reset();
	glfwPollEvents();
	updateCallback();
	glfwSwapBuffers(window);
}

bool Window::shouldClose()
{
	return glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) != 0;
}
