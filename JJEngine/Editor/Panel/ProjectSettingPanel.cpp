#include "ProjectSettingPanel.h"
#include <imgui.h>
#include <Core/Asset/Manager/AssetManager.h>
#include <Core/Asset/Metadata.h>
#include <Core/Application.h>

ProjectSettingPanel::ProjectSettingPanel(PanelMessenger& mg): messenger(mg)
{
}

void ProjectSettingPanel::OnImGuiRender()
{
	if (isOpened == true) 
	{
		ImGui::Begin("Project setting", &isOpened, 0/*ImGuiWindowFlags*/);
		ImGui::Text("Scene setting");
		std::shared_ptr<AssetManager> assetManager{Application::Instance().GetAssetManager()};
		std::map< AssetHandle, std::string>& projectScenes = assetManager->projectScenes;
		std::vector<AssetHandle> destroyList{};
		
		for (auto &s:projectScenes) {
			ImGui::Text("Scene name: ");
			ImGui::SameLine();
			std::array<char, 256> inputText{ 0, };
			std::copy(s.second.begin(), s.second.end(), inputText.data());
			if (ImGui::InputText("##", inputText.data(), inputText.size())) {
				s.second = inputText.data();
			}
			ImGui::SameLine();
			ImGui::Text("Scene path: ");
			ImGui::SameLine();
			ImGui::Text(assetManager->GetMetadata(s.first)->path.string().data());
			//TODO: erase button
		}
		for (auto&d:destroyList) {
			projectScenes.erase(d);
		}

		std::unordered_set<AssetHandle> scenes = assetManager->GetAllAssetsWithType(AssetType::Scene);
		unsigned Asize{ static_cast<unsigned>(scenes.size()) };
		std::vector < std::string > items(Asize);
		unsigned nitem = 0;
		static unsigned curr_item_idx{};
		std::string currPath{"None"};
		AssetHandle currhandle{};
		for (auto& h : scenes) {
			items[nitem] = assetManager->GetMetadata(h)->path.string();
			if (curr_item_idx == nitem + 1) {
				currPath = items[nitem];
				currhandle = h;
			}
			++nitem;
		}
		nitem = 0;

		const char* combo_preview_value = currPath.c_str();
		if (ImGui::BeginCombo("Scene path", combo_preview_value))
		{
			nitem = 0;
			for (auto& h : scenes)
			{
				const bool is_selected = (curr_item_idx == nitem + 1);
				if (ImGui::Selectable(items[nitem].c_str(), is_selected)) {
					curr_item_idx = nitem + 1;
					currhandle = h;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();

				++nitem;
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		if (ImGui::Button("Enroll new scene") == true) {
			if (currhandle != AssetHandle{})
				projectScenes[currhandle] = "None";
		}
		ImGui::Separator();
		ImGui::End();
	}
}

void ProjectSettingPanel::SetIsOpened(bool TF)
{
	isOpened = TF;
}
