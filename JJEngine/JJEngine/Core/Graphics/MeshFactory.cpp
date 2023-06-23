#include "MeshFactory.h"
#include "Mesh.h"

struct Face
{
	unsigned i1, i2, i3;
};

std::shared_ptr<Mesh> MeshFactory::CreateBox(const glm::vec3& size)
{
	std::vector<Vertex> vertices;
	vertices.resize(8);
	vertices[0].position = { -size.x / 2.0f, -size.y / 2.0f,  size.z / 2.0f };
	vertices[1].position = { size.x / 2.0f, -size.y / 2.0f,  size.z / 2.0f };
	vertices[2].position = { size.x / 2.0f,  size.y / 2.0f,  size.z / 2.0f };
	vertices[3].position = { -size.x / 2.0f,  size.y / 2.0f,  size.z / 2.0f };
	vertices[4].position = { -size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f };
	vertices[5].position = { size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f };
	vertices[6].position = { size.x / 2.0f,  size.y / 2.0f, -size.z / 2.0f };
	vertices[7].position = { -size.x / 2.0f,  size.y / 2.0f, -size.z / 2.0f };

	vertices[0].normal = { -1.0f, -1.0f,  1.0f };
	vertices[1].normal = { 1.0f, -1.0f,  1.0f };
	vertices[2].normal = { 1.0f,  1.0f,  1.0f };
	vertices[3].normal = { -1.0f,  1.0f,  1.0f };
	vertices[4].normal = { -1.0f, -1.0f, -1.0f };
	vertices[5].normal = { 1.0f, -1.0f, -1.0f };
	vertices[6].normal = { 1.0f,  1.0f, -1.0f };
	vertices[7].normal = { -1.0f,  1.0f, -1.0f };

	std::vector<Face> indices;
	indices.resize(12);
	indices[0] = { 0, 1, 2 };
	indices[1] = { 2, 3, 0 };
	indices[2] = { 1, 5, 6 };
	indices[3] = { 6, 2, 1 };
	indices[4] = { 7, 6, 5 };
	indices[5] = { 5, 4, 7 };
	indices[6] = { 4, 0, 3 };
	indices[7] = { 3, 7, 4 };
	indices[8] = { 4, 5, 1 };
	indices[9] = { 1, 0, 4 };
	indices[10] = { 3, 2, 6 };
	indices[11] = { 6, 7, 3 };

	std::shared_ptr<Mesh> to_return = std::make_shared<Mesh>();


	to_return->vertices = vertices;
	
	to_return->indices.reserve(indices.size() * 3);
	for (auto&[i1,i2,i3] : indices)
	{
		to_return->indices.push_back(i1);
		to_return->indices.push_back(i2);
		to_return->indices.push_back(i3);
	}


	to_return->VBO = VertexBuffer::CreateVertexBuffer(sizeof(Vertex) * vertices.size());
	to_return->VBO->SetData(sizeof(Vertex) * vertices.size(), vertices.data());
	to_return->VBO->SetDataTypes({
			{0, DataType::Float3},//location=0, pos
			{1, DataType::Float3},//location=1, normal
			{2, DataType::Float2},//location=2, uv
	});

	to_return->EBO = IndexBuffer::CreateIndexBuffer(sizeof(Face) * indices.size());
	to_return->EBO->SetData(sizeof(Face) * indices.size(), indices.data());
	return to_return;
}
