#pragma once
class Application;
class Time
{
	friend Application;
public:
	static float GetDelta();
private:
	static void Update();
	static float dt;
	static float m_LastTime;
};


