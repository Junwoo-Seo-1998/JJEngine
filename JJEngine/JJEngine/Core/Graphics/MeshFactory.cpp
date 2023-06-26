#include "MeshFactory.h"
#include "Mesh.h"

struct Face
{
	unsigned i1, i2, i3;
};

std::shared_ptr<Mesh> MeshFactory::CreateBox(const glm::vec3& size)
{
	const glm::vec3 LeftFrontDown = { -size.x / 2.0f, -size.y / 2.0f,  size.z / 2.0f };
	const glm::vec3 RightFrontDown = { size.x / 2.0f, -size.y / 2.0f,  size.z / 2.0f };
	const glm::vec3 RightFrontUp = { size.x / 2.0f,  size.y / 2.0f,  size.z / 2.0f };
	const glm::vec3 LeftFrontUp = { -size.x / 2.0f,  size.y / 2.0f,  size.z / 2.0f };

	const glm::vec3 LeftBackDown = { -size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f };
	const glm::vec3 RightBackDown = { size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f };
	const glm::vec3 RightBackUp = { size.x / 2.0f,  size.y / 2.0f, -size.z / 2.0f };
	const glm::vec3 LeftBackUp = { -size.x / 2.0f,  size.y / 2.0f, -size.z / 2.0f };

	std::vector<Vertex> vertices;
	vertices.resize(24);

	//front
	vertices[0].position = LeftFrontDown;
	vertices[1].position = RightFrontDown;
	vertices[2].position = RightFrontUp;
	vertices[3].position = LeftFrontUp;

	//back
	vertices[4].position = LeftBackDown;
	vertices[5].position = RightBackDown;
	vertices[6].position = RightBackUp;
	vertices[7].position = LeftBackUp;

	//left
	vertices[8].position = LeftFrontDown;
	vertices[9].position = LeftBackDown;
	vertices[10].position = LeftBackUp;
	vertices[11].position = LeftFrontUp;

	//right
	vertices[12].position = RightFrontDown;
	vertices[13].position = RightBackDown;
	vertices[14].position = RightBackUp;
	vertices[15].position = RightFrontUp;

	//up
	vertices[16].position = LeftFrontUp;
	vertices[17].position = RightFrontUp;
	vertices[18].position = RightBackUp;
	vertices[19].position = LeftBackUp;

	//down
	vertices[20].position = LeftFrontDown;
	vertices[21].position = RightFrontDown;
	vertices[22].position = RightBackDown;
	vertices[23].position = LeftBackDown;

	//front
	constexpr glm::vec3 front = { 0.f, 0.f, 1.f };
	vertices[0].normal = front;
	vertices[1].normal = front;
	vertices[2].normal = front;
	vertices[3].normal = front;

	//back
	constexpr glm::vec3 back = { 0.f, 0.f, -1.f };
	vertices[4].normal = back;
	vertices[5].normal = back;
	vertices[6].normal = back;
	vertices[7].normal = back;

	//left
	constexpr glm::vec3 left = { -1.f, 0.f, 0.f };
	vertices[8].normal = left;
	vertices[9].normal = left;
	vertices[10].normal = left;
	vertices[11].normal = left;

	//right
	constexpr glm::vec3 right = { 1.f, 0.f, 0.f };
	vertices[12].normal = right;
	vertices[13].normal = right;
	vertices[14].normal = right;
	vertices[15].normal = right;

	//up
	constexpr glm::vec3 up = { 0.f, 1.f, 0.f };
	vertices[16].normal = up;
	vertices[17].normal = up;
	vertices[18].normal = up;
	vertices[19].normal = up;

	//down
	constexpr glm::vec3 down = { 0.f, -1.f, 0.f };
	vertices[20].normal = down;
	vertices[21].normal = down;
	vertices[22].normal = down;
	vertices[23].normal = down;

	std::vector<Face> faces;
	faces.resize(12);

	//front
	faces[0] = { 0, 1, 2 };
	faces[1] = { 2, 3, 0 };

	//back
	faces[2] = { 6,5,4 };
	faces[3] = { 7,6,4 };

	//left
	faces[4] = { 10, 9, 8 };
	faces[5] = { 10, 8, 11 };

	//right
	faces[6] = { 12,13,14 };
	faces[7] = { 12,14,15 };

	//up
	faces[8] = { 16,17,18 };
	faces[9] = { 16,18,19 };

	//down
	faces[10] = { 22,21,20 };
	faces[11] = { 23,22,20 };

	std::shared_ptr<Mesh> to_return = std::make_shared<Mesh>();


	to_return->vertices = vertices;
	
	to_return->indices.reserve(faces.size() * 3);
	for (auto&[i1,i2,i3] : faces)
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

	to_return->EBO = IndexBuffer::CreateIndexBuffer(sizeof(Face) * faces.size());
	to_return->EBO->SetData(sizeof(Face) * faces.size(), faces.data());
	return to_return;
}

std::shared_ptr<Mesh> MeshFactory::CreateSkyCube(const glm::vec3& size)
{
	glm::vec3 posMin = -size / 2.f;
	glm::vec3 posMax = size / 2.f;
	Mesh square;

	square.vertices.push_back(Vertex{ posMin });
	square.vertices.push_back(Vertex{ glm::vec3{ posMax.x, posMin.y, posMin.z } });
	square.vertices.push_back(Vertex{glm::vec3{ posMax.x, posMax.y, posMin.z }});
	square.vertices.push_back(Vertex{glm::vec3{ posMin.x, posMax.y, posMin.z }});
							  												  
	square.vertices.push_back(Vertex{glm::vec3{ posMin.x, posMin.y, posMax.z }});
	square.vertices.push_back(Vertex{glm::vec3{ posMax.x, posMin.y, posMax.z }});
	square.vertices.push_back(Vertex{ posMax });
	square.vertices.push_back(Vertex{glm::vec3{ posMin.x, posMax.y, posMax.z }});

	square.indices.push_back(0);
	square.indices.push_back(1);
	square.indices.push_back(2);
	square.indices.push_back(2);
	square.indices.push_back(3);
	square.indices.push_back(0);


	square.indices.push_back(1);
	square.indices.push_back(5);
	square.indices.push_back(6);
	square.indices.push_back(6);
	square.indices.push_back(2);
	square.indices.push_back(1);


	square.indices.push_back(5);
	square.indices.push_back(4);
	square.indices.push_back(7);
	square.indices.push_back(7);
	square.indices.push_back(6);
	square.indices.push_back(5);

	square.indices.push_back(4);
	square.indices.push_back(0);
	square.indices.push_back(3);
	square.indices.push_back(3);
	square.indices.push_back(7);
	square.indices.push_back(4);

	square.indices.push_back(3);
	square.indices.push_back(2);
	square.indices.push_back(6);
	square.indices.push_back(6);
	square.indices.push_back(7);
	square.indices.push_back(3);

	square.indices.push_back(4);
	square.indices.push_back(5);
	square.indices.push_back(1);
	square.indices.push_back(1);
	square.indices.push_back(0);
	square.indices.push_back(4);

	square.VBO = VertexBuffer::CreateVertexBuffer(sizeof(Vertex) * square.vertices.size());
	square.VBO->SetData(sizeof(Vertex) * square.vertices.size(), square.vertices.data());
	square.VBO->SetDataTypes({
			{0, DataType::Float3},//location=0, pos
			{1, DataType::Float3},//location=1, normal
			{2, DataType::Float2},//location=2, uv
		});

	square.EBO = IndexBuffer::CreateIndexBuffer(sizeof(unsigned) * square.indices.size());
	square.EBO->SetData(sizeof(unsigned) * square.indices.size(), square.indices.data());

	return std::make_shared<Mesh>(square);
}
