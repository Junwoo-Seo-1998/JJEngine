#include "AssetBrowserPanel.h"
#include <Core/Utils/File.h>
#include <imgui.h>
#include <Core/Utils/Log.h>
void AssetBrowserPanel::OnImGuiRender()
{
	if (ImGui::Button("<-") == true) {
		if (nowDirectory != AssetDirectory) {
			nowDirectory = nowDirectory.parent_path();
		}
	}
	std::vector<std::filesystem::path> files = File::GetFileLists(std::filesystem::path{ nowDirectory });
	for (auto& p : files)
	{	
		if (std::filesystem::is_directory(p) == true) {
			if (ImGui::Button(p.stem().string().c_str()) == true) {
				nowDirectory /= p.filename();
			}
		}
		else {
			ImGui::Text(p.filename().string().c_str());
		}
	}
}
