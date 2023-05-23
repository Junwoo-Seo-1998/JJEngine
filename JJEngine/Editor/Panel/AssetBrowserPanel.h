#pragma once
#include <filesystem>
class AssetBrowserPanel {
	std::filesystem::path AssetDirectory{ "." };
	std::filesystem::path nowDirectory{"."};
public:
	//AssetBrowserPanel() = default;

	void OnImGuiRender();
};