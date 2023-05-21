/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Daehyeon Kim
Platform: x64
Date: 05/21s/2023
End Header-------------------------------------------------------- */
#include "ImGuiSubWindow.h"
#include "ImGuiCommand.h"
#include <imgui.h>
void ImGuiSubWindow::Push_ImGuiCommand(std::function<void()> command)
{
	contents.emplace_back(new ImGuiCommand{command});
}

void ImGuiSubWindow::Update()
{
	ImGui::Begin(windowName.c_str());
	for (auto& c: contents) {
		c->Update();
	}
	ImGui::End();
}

ImGuiSubWindow::ImGuiSubWindow(std::string window_name):windowName(window_name)
{
}

ImGuiSubWindow::~ImGuiSubWindow()
{
	for (auto& c : contents) {
		delete c;
	}
	contents.clear();
}
