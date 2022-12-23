/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Dahyeon Kim
Platform: x64
Date: 12/20/2022
End Header-------------------------------------------------------- */
#include "VertexBuffer.h"
#include "glad.h"
VertexBuffer::~VertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &m_Buffer);
}


void VertexBuffer::SetData(int size, const void* data, unsigned offset)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

VertexBuffer::VertexBuffer(int byte_size)
{
	CreateBuffer(byte_size, nullptr);
}

void VertexBuffer::CreateBuffer(int size, const void* data)
{
	glCreateBuffers(1, &m_Buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

std::shared_ptr<VertexBuffer> VertexBuffer::CreateVertexBuffer(int byte_size)
{
	return std::shared_ptr<VertexBuffer>(new VertexBuffer{ byte_size });
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
}

void VertexBuffer::UnBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
