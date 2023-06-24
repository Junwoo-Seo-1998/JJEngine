#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include <cmath>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
class SceneRenderer;
class MeshFactory;
class Material;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

typedef std::vector<Vertex> VertexBufferType;
typedef std::vector<unsigned> IndexBufferType;


class Mesh
{
	friend SceneRenderer;
	friend MeshFactory;
public:
	[[nodiscard]] std::shared_ptr<Material> GetMaterial() { return m_BaseMaterial; }

	[[nodiscard]] VertexBufferType& GetVertices() { return vertices; }
	[[nodiscard]] unsigned int GetNumOfVertices() const { return static_cast<unsigned int>(vertices.size()); }
	[[nodiscard]] IndexBufferType& GetIndices() { return indices; }
	[[nodiscard]] unsigned int GetNumOfIndices() const { return static_cast<unsigned int>(indices.size()); }

	
	[[nodiscard]] std::shared_ptr<VertexBuffer> GetMeshVBO() { return VBO; }
	[[nodiscard]] std::shared_ptr<IndexBuffer> GetMeshEBO() { return EBO; }

private:
	VertexBufferType vertices;
	IndexBufferType indices;
	std::shared_ptr<VertexBuffer> VBO;
	std::shared_ptr<IndexBuffer> EBO;

	std::shared_ptr<Material> m_BaseMaterial;

public:
	static Mesh CreateSphere(int stacks, int slices, float radius, glm::vec3 center) {
		Mesh mesh;
		for (int i = 0; i <= stacks; ++i) {
			float row = (float)i / (float)stacks;
			float beta = 3.14159 * (row - 0.5f);
			for (int j = 0; j <= slices; ++j) {
				float  col = (float)j / (float)slices;
				float  alpha = col * 3.14159 * 2.f;
				Vertex v;
				v.position = center + glm::vec3(radius * sin(alpha) * cos(beta), radius * sin(beta), radius * cos(alpha) * cos(beta));
				v.normal = glm::vec3(v.position.x, v.position.y, v.position.z) / radius;
				mesh.vertices.push_back(v);
			}
		}
		BuildIndexBuffer(stacks, slices, mesh);
		return mesh;
	}


	static Mesh CreatePlane(int stacks, int slices)
	{
		Mesh mesh;
		for (int stack = 0; stack <= stacks; ++stack)
		{
			float row = (float)stack / stacks;
			for (int slice = 0; slice <= slices; ++slice)
			{
				float col = (float)slice / slices;
				Vertex v;
				v.position = glm::vec3(col - 0.5f, 0.f, 0.5f - row);
				v.normal = glm::vec3(0.0f, 1.0f, 0.0f);
				mesh.vertices.push_back(v);
			}
		}
		BuildIndexBuffer(stacks, slices, mesh);
		return mesh;
	}


private:
	static constexpr float EPSILON = std::numeric_limits<float>::epsilon();

	static bool DegenerateTri(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) {
		return (glm::distance(v0, v1) < EPSILON ||
			glm::distance(v1, v2) < EPSILON ||
			glm::distance(v2, v0) < EPSILON);
	}

	static void BuildIndexBuffer(int stacks, int slices, Mesh& mesh)
	{
		int p0 = 0, p1 = 0, p2 = 0;
		int p3 = 0, p4 = 0, p5 = 0;
		const int stride = slices + 1;
		for (int i = 0; i < stacks; ++i) {
			int current_row = i * stride;
			for (int j = 0; j < slices; ++j) {
				p0 = current_row + j;
				p1 = p0 + 1;
				p2 = p1 + stride;
				if (!DegenerateTri(mesh.vertices[p0].position, mesh.vertices[p1].position, mesh.vertices[p2].position)) {
					mesh.indices.push_back(p0);
					mesh.indices.push_back(p1);
					mesh.indices.push_back(p2);

				}
				p3 = p2;
				p4 = p3 - 1;
				p5 = p0;
				if (!DegenerateTri(mesh.vertices[p3].position, mesh.vertices[p4].position, mesh.vertices[p5].position)) {
					mesh.indices.push_back(p3);
					mesh.indices.push_back(p4);
					mesh.indices.push_back(p5);
				}
			}
		}
	}

};

class Model
{
public:
	[[nodiscard]] std::vector<std::shared_ptr<Mesh>>& GetMeshes() { return meshes; }
	std::vector<std::shared_ptr<Mesh>>::iterator begin() { return meshes.begin(); }
	std::vector<std::shared_ptr<Mesh>>::iterator end() { return meshes.end(); }
	std::vector<std::shared_ptr<Mesh>>::reverse_iterator rbegin() { return meshes.rbegin(); }
	std::vector<std::shared_ptr<Mesh>>::reverse_iterator rend() { return meshes.rend(); }

private:
	std::vector<std::shared_ptr<Mesh>> meshes;
};