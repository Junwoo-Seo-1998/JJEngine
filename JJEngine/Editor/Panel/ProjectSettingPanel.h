#pragma once
#include "PanelMessenger.h"
class ProjectSettingPanel {
	PanelMessenger& messenger;
	bool isOpened{false};
public:
	ProjectSettingPanel(PanelMessenger& mg);
	void OnImGuiRender();
	void SetIsOpened(bool TF);
};