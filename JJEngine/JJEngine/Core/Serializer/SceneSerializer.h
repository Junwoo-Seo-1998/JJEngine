#pragma once
#include <memory>
#include <string>
class Scene;
class SceneSerializer {
	std::shared_ptr<Scene> scene{};
public:
	SceneSerializer(std::shared_ptr<Scene> sc);
	void Serialize(const std::string filePath);
	bool Deserialize(const std::string filePath);
};