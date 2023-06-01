#pragma once
#include <string>

class Event;
class Layer
{
public:
	Layer(const std::string& name = "Layer")
		:m_Name(name) {}
	virtual ~Layer() {}

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnStart() {}
	virtual void OnUpdate() {}
	virtual void OnPreRender() {}
	virtual void OnRender() {}
	virtual void OnPostRender() {}
	virtual void OnImGuiRender() {}

	virtual void OnEvent(Event& event) {}

	inline const std::string& Name() const { return m_Name; }
protected:
	std::string m_Name;
};
