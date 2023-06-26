#pragma once
#include <memory>
#include <glm/vec3.hpp>

class Mesh;

class MeshFactory
{
public:
	//todo : make it return asset handle later
	static std::shared_ptr<Mesh> CreateBox(const glm::vec3& size);

};
