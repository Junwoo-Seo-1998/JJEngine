#include "IndexBuffer.h"
#include <glad.h>


IndexBuffer::~IndexBuffer()
{
}

std::shared_ptr<IndexBuffer> IndexBuffer::CreateIndexBuffer(int byte_size)
{
	return std::shared_ptr<IndexBuffer>(new IndexBuffer{ byte_size });
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
}

void IndexBuffer::BindToVertexArray() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
}

void IndexBuffer::UnBind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::SetData(int size, const void* data, unsigned offset)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

IndexBuffer::IndexBuffer(int byte_size)
	:m_Buffer(0)
{
	CreateBuffer(byte_size, nullptr);
}

void IndexBuffer::CreateBuffer(unsigned size, const void* data)
{
	glGenBuffers(1, &m_Buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
