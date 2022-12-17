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

Window::Window()
{
	if (!glfwInit())
	{
		std::cout << "GLFW init Failed" << std::endl;
		glfwTerminate();

		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	window = glfwCreateWindow(windowWidth, windowHeight, "JJEngine",
		nullptr, // which monitor (if full-screen mode)
		nullptr); // if sharing context with another window
	if (!window) {
		std::cout << "GLFW can't make window";
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	int err = gladLoadGL();
	if (err != 1) {
		std::cerr << "Unable to initialize Glad - error: " << " abort program" << std::endl;
		glfwTerminate();
		return;
	}

	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

Window::~Window()
{
	glfwTerminate();
}

void Window::update()
{
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Window::shouldClose()
{
	return glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) != 0;
}
