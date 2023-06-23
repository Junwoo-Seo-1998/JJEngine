#pragma once
class Mesh;
struct MeshComponent
{
	bool reserved;
	std::shared_ptr<Mesh> mesh;
};
