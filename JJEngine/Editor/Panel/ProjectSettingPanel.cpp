#include "ProjectSettingPanel.h"
#include <imgui.h>

ProjectSettingPanel::ProjectSettingPanel(PanelMessenger& mg): messenger(mg)
{
}

void ProjectSettingPanel::OnImGuiRender()
{
	if (isOpened == true) 
	{
		ImGui::Begin("Project setting", &isOpened, 0/*ImGuiWindowFlags*/);

		ImGui::End();
	}
}

void ProjectSettingPanel::SetIsOpened(bool TF)
{
	isOpened = TF;
}
