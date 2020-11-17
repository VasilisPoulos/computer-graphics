#pragma once
#include "ErrorHandling.h"
class VertexBuffer
{
private:
	GLuint m_VertexBufferID;
public:
	VertexBuffer(const GLvoid* data, GLsizeiptr size, GLenum usage);
	~VertexBuffer();
	void bind() const;
	void unbind() const;
};

