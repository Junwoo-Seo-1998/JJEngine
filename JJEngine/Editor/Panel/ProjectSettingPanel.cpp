#include "ProjectSettingPanel.h"
#include <imgui.h>
#include <Core/Asset/Manager/AssetManager.h>
#include <Core/Asset/Metadata.h>
#include <Core/Application.h>
#include <Core/Serializer/GameDataSerializer.h>

ProjectSettingPanel::ProjectSettingPanel(PanelMessenger& mg): messenger(mg)
{
}

void ProjectSettingPanel::OnImGuiRender()
{
	if (isOpened == true) 
	{
		std::shared_ptr<AssetManager> assetManager{Application::Instance().GetAssetManager()};

		ImGui::Begin("Project setting", &isOpened, 0/*ImGuiWindowFlags*/);
		std::unordered_set<AssetHandle> scenes = assetManager->GetAllAssetsWithType(AssetType::Scene);
		unsigned Asize{ static_cast<unsigned>(scenes.size()) };
		std::vector < std::string > items(Asize);
		unsigned nitem = 0;
		AssetHandle startScene{};
		for (auto& h : scenes) {
			items[nitem] = assetManager->GetMetadata(h)->path.string();
			++nitem;
		}
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Text("Scene setting section");
		ImGui::Separator();
		{
			ImGui::Text("Start scene: ");
			ImGui::SameLine();
			static unsigned curr_item_idx_startScene{};
			for (auto& h : scenes) {
				if (curr_item_idx_startScene == nitem) {
					startScene = h;
				}
				++nitem;
			}
			const char* combo_preview_value = items[curr_item_idx_startScene].c_str();
			if (ImGui::BeginCombo("Choose start scene", combo_preview_value))
			{
				nitem = 0;
				for (auto& h : scenes)
				{
					const bool is_selected = (curr_item_idx_startScene == nitem + 1);
					if (ImGui::Selectable(items[nitem].c_str(), is_selected)) {
						curr_item_idx_startScene = nitem;
						startScene = h;
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();

					++nitem;
				}
				ImGui::EndCombo();
			}
			
		}
		ImGui::Separator();
		std::map< AssetHandle, std::string>& projectScenes = assetManager->projectScenes;
		std::vector<AssetHandle> destroyList{};
		
		for (auto &s:projectScenes) {
			ImGui::Text("Scene name: ");
			ImGui::SameLine();
			std::array<char, 256> inputText{ 0, };
			std::copy(s.second.begin(), s.second.end(), inputText.data());
			if (ImGui::InputText(std::format("##{}", to_string(s.first)).c_str(), inputText.data(), inputText.size())) {
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

		nitem = 0;
		static unsigned curr_item_idx{};
		std::string currPath{"None"};
		AssetHandle currhandle{};
		for (auto& h : scenes) {
			if (curr_item_idx == nitem + 1) {
				currPath = items[nitem];
				currhandle = h;
			}
			++nitem;
		}

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
		ImGui::Separator();
		ImGui::Separator();
		//if (ImGui::Button("Make Project") == true) {
		//}
		//if (ImGui::Button("Opwn Project") == true) {
		//}
		if (ImGui::Button("Save Project") == true) {
			GameDataSerializer g_data_serializer{"./TestGameData.GData"};
			g_data_serializer.startScene = startScene;
			g_data_serializer.projectScenes = projectScenes;
			g_data_serializer.Deserialize();
		}
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Separator();
		ImGui::End();
	}
}

void ProjectSettingPanel::SetIsOpened(bool TF)
{
	isOpened = TF;
}
