#pragma once
#include <string>

class Layer
{
public:
	Layer(const std::string& name = "Layer");
	virtual ~Layer();

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate() {}
	virtual void OnImGuiRender() {}

	inline const std::string& Name() const { return m_Name; }
protected:
	std::string m_Name;
};
