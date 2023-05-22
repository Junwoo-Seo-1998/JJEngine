#include "Time.h"
#include "GLFW/glfw3.h"
float Time::dt = 0.f;
float Time::m_LastTime = 0.f;

float Time::GetDelta()
{
	return dt;
}

void Time::Update()
{
	const float time = static_cast<float>(glfwGetTime());
	dt = time - m_LastTime;
	m_LastTime = time;
}
