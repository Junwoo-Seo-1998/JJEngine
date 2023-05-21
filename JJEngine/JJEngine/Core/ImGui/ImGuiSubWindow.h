/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Daehyeon Kim
Platform: x64
Date: 05/21s/2023
End Header-------------------------------------------------------- */
#pragma once
#include "ImGuiContent.h"
#include <vector>
#include <functional>
#include <string>

class ImGuiSubWindow {
	std::vector<ImGuiContent*> contents{}; // change to entt registry?
	std::string windowName{};
public:
	ImGuiSubWindow(std::string window_name);
	~ImGuiSubWindow();
	void Push_ImGuiCommand(std::function<void()> command);
	
	void Update();
};