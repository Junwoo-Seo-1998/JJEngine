/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Daehyeon Kim
Platform: x64
Date: 05/21s/2023
End Header-------------------------------------------------------- */
#pragma once
#include <functional>
#include "ImGuiContent.h"

class ImGuiCommand : public ImGuiContent {
	std::function<void()> command{};
public:
	ImGuiCommand(std::function<void()> c);
	void Update()override;
};