#include "VertexBuffer.h"
VertexBuffer::VertexBuffer(const GLvoid* data, GLsizeiptr size, GLenum usage) {
	GLCall(glGenBuffers(1, &m_VertexBufferID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, usage);)
}

VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1, &m_VertexBufferID));
}

void VertexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID));
}

void VertexBuffer::unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}