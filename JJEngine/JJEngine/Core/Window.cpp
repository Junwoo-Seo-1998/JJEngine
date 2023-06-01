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

#include "Application.h"
#include "Event/EventManager.h"
#include "Event/WindowEvent.h"
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

	glfwSetWindowUserPointer(window, this);

	glfwSetWindowSizeCallback(window, GLFWResizeCallback);
	glfwSetKeyCallback(window, GLFWKeyCallback);
	glfwSetMouseButtonCallback(window, GLFWMouseCallback);
	glfwSetCursorPosCallback(window, GLFWMousePositionCallback);

	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

Window::~Window()
{
	glfwTerminate();
}

void Window::update(std::function<void()> updateCallback)
{
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	Input::Reset();
	glfwPollEvents();
	Input::PostEventUpdate();
	updateCallback();
	glfwSwapBuffers(window);
}

bool Window::shouldClose()
{
	return glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) != 0;
}

GLFWwindow* Window::GetGLFWWindow()
{
	return window;
}

std::tuple<int, int> Window::GetWidthAndHeight()
{
	return { windowWidth, windowHeight };
}

void Window::SetWindowSize(int width, int height)
{
	windowWidth = width;
	windowHeight = height;
}

void Window::GLFWKeyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
	if (key < 0)
		return;
	switch (action)
	{
	case GLFW_PRESS:
	{
		Input::SetKey(static_cast<KeyCode>(key), true);
		break;
	}
	case GLFW_RELEASE:
	{
		Input::SetKey(static_cast<KeyCode>(key), false);
		break;
	}
	case GLFW_REPEAT:
	{
		Input::SetKey(static_cast<KeyCode>(key), true);
		break;
	}
	default:
		break;
	}
}

void Window::GLFWMouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
	{
		Input::SetMouseButton(static_cast<Mouse>(button), true);
		break;
	}
	case GLFW_RELEASE:
	{
		Input::SetMouseButton(static_cast<Mouse>(button), false);
		break;
	}
	default:
		break;
	}
}

void Window::GLFWMousePositionCallback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	Input::SetMousePosition(xpos, ypos);
}

void Window::GLFWResizeCallback(GLFWwindow* window, int width, int height)
{
	Window& windowClass = *(Window*)glfwGetWindowUserPointer(window);
	windowClass.SetWindowSize(width, height);
	Application::Instance().GetEventManager()->AddEvent(std::make_shared<WindowResizeEvent>(width, height));
}
