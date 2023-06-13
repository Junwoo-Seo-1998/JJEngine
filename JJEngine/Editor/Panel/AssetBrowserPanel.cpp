#include "AssetBrowserPanel.h"
#include <Core/Utils/File.h>
#include <imgui.h>
#include <Core/Utils/Log.h>
#include <Core/Graphics/Texture.h>

AssetBrowserPanel::AssetBrowserPanel()
{

}
void AssetBrowserPanel::Set()
{
	Folder_texture = Texture::CreateTexture(File::ReadImageToTexture("Resources/Textures/FolderImage.png"));
	File_texture = Texture::CreateTexture(File::ReadImageToTexture("Resources/Textures/FileImage.png"));
}
void AssetBrowserPanel::SetSelectedFileFunc(std::function<void(std::filesystem::path)> func)
{
	setSelectedFile = func;
}
void AssetBrowserPanel::OnImGuiRender_ResorceHierarchy(std::filesystem::path p)
{
	std::vector<std::filesystem::path> files = File::GetFileLists(p);
	for (auto& f : files) {
		ImGuiTreeNodeFlags flag{};
		bool isDirectory{ std::filesystem::is_directory(f) };
		if (isDirectory == true) {
			flag =  ImGuiTreeNodeFlags_OpenOnArrow;
		}
		else {
			flag = ImGuiTreeNodeFlags_Leaf;
		}

		auto name = f.filename().string() + "##" + f.string();
		bool opened = ImGui::TreeNodeEx(name.c_str(), flag);

		if (ImGui::IsItemClicked() == true && isDirectory == true) {
			nowDirectory = f;
		}

		if (opened == true) {
			if (isDirectory == true) {
				OnImGuiRender_ResorceHierarchy(f);
			}
			ImGui::TreePop();
		}
	}
}
void AssetBrowserPanel::OnImGuiRender()
{
	ImGui::Begin("Resorce hierarchy");
	OnImGuiRender_ResorceHierarchy(".");
	ImGui::End();


	if (ImGui::Button("<-") == true) {
		if (nowDirectory != AssetDirectory) {
			nowDirectory = nowDirectory.parent_path();
		}
	}
	static float padding{16.f};
	static float thumbnailSize{128.f};
	float cellSize{padding + thumbnailSize};

	float panelWidth{ ImGui::GetContentRegionAvail().x};
	int columnCount{(int)(panelWidth/cellSize)};
	ImGui::Columns(columnCount > 0 ? columnCount : 1,0,false);

	std::vector<std::filesystem::path> files = File::GetFileLists(std::filesystem::path{ nowDirectory });
	for (auto& p : files)
	{	
		bool isDirectory{ std::filesystem::is_directory(p) };
		if (isDirectory == true) {
			ImGui::ImageButton((ImTextureID)Folder_texture->GetTextureID(), { thumbnailSize,thumbnailSize }, { 0,1 }, { 1,0 });
		}
		else {
			ImGui::ImageButton((ImTextureID)File_texture->GetTextureID(), { thumbnailSize,thumbnailSize }, { 0,1 }, { 1,0 });
		}
		bool isDoubliClicked{ ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) };
		bool isHovering{ ImGui::IsItemHovered()};
		if (isHovering == true && isDoubliClicked == true) {
			if (isDirectory == true) {
				nowDirectory /= p.filename();
			}
			else {
				setSelectedFile(nowDirectory / p.filename());
			}
		}
		ImGui::Text(p.filename().string().c_str());
		ImGui::NextColumn();
	}

	ImGui::Columns(1);
}
