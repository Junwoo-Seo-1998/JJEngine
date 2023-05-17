#pragma once
#include <memory>
class ImGuiRenderer
{
public:
	static std::shared_ptr<ImGuiRenderer> Instance();

	~ImGuiRenderer() = default;
	void Init(void* window);
	void GuiBegin();
	void GuiDrawDockSpace();
	void GuiEnd();
	void Shutdown();
private:
	ImGuiRenderer() = default;
	static std::shared_ptr<ImGuiRenderer> m_Instance;
};