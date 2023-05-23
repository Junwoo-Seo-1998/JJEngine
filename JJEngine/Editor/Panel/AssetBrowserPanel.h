#pragma once
#include <filesystem>
class Texture;
class AssetBrowserPanel {
	std::filesystem::path AssetDirectory{ "." };
	std::filesystem::path nowDirectory{"."};
	std::shared_ptr<Texture> Folder_texture;
	std::shared_ptr<Texture> File_texture;
public:
	AssetBrowserPanel();
	void Set();
	void OnImGuiRender();
};