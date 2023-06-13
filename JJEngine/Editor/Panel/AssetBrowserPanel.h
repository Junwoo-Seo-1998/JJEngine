#pragma once
#include <functional>
#include <filesystem>
class Texture;
class AssetBrowserPanel {
	std::filesystem::path AssetDirectory{ "." };
	std::filesystem::path nowDirectory{"."};
	std::shared_ptr<Texture> Folder_texture;
	std::shared_ptr<Texture> File_texture;

	std::function<void(std::filesystem::path)> setSelectedFile;
public:
	AssetBrowserPanel();
	void Set();
	void SetSelectedFileFunc(std::function<void(std::filesystem::path)> func);
	void OnImGuiRender_ResorceHierarchy(std::filesystem::path p);
	void OnImGuiRender();
};