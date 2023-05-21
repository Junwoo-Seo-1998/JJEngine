/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Daehyeon Kim
Platform: x64
Date: 05/21s/2023
End Header-------------------------------------------------------- */
#include "ImGuiCommand.h"

ImGuiCommand::ImGuiCommand(std::function<void()> c): command(c)
{
}

void ImGuiCommand::Update()
{
	command();
}
