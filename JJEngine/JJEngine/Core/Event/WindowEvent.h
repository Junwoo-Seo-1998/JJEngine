#pragma once
#include "Event.h"
#include <sstream>
class WindowResizeEvent : public Event
{
public:
	//add this always
	EVENT_CLASS_TYPE(WindowResize);
	WindowResizeEvent(unsigned int width, unsigned int height)
		:m_Width(width), m_Height(height)
	{
	}
	
	inline unsigned int GetWidth() const { return m_Width; }
	inline unsigned int GetHeight() const { return m_Height; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
		return ss.str();
	}
	

private:
	unsigned int m_Width, m_Height;
};
