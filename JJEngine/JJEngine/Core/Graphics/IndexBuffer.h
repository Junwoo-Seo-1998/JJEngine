#pragma once
#include <memory>
#include <vector>
class IndexBuffer
{
public:
	IndexBuffer() = delete;
	virtual ~IndexBuffer();
	static std::shared_ptr<IndexBuffer> CreateIndexBuffer(int byte_size);
	void Bind() const;
	void BindToVertexArray() const;
	void UnBind() const;
	void SetData(int size, const void* data, unsigned offset = 0);
private:
	IndexBuffer(int byte_size);
	void CreateBuffer(unsigned size, const void* data);

	unsigned m_Buffer;
};
