#pragma once
#include <functional>
#include <filesystem>
#include "PanelMessenger.h"
class Texture;
class AssetBrowserPanel {
	PanelMessenger& messenger;

	std::filesystem::path AssetDirectory{ "." };
	std::filesystem::path nowDirectory{"."};
	std::shared_ptr<Texture> Folder_texture;
	std::shared_ptr<Texture> File_texture;

	void OpenFile(std::string path);

public:
	AssetBrowserPanel(PanelMessenger& mg);
	void Set();
	void OnImGuiRender_ResorceHierarchy(std::filesystem::path p);
	void OnImGuiRender();
};