#pragma once
#include "ErrorHandling.h"
#include "VertexBuffer.h"
class VertexArray
{
private:
	GLuint m_VertexArrayID;
public:
	VertexArray();
	~VertexArray();

	void bind() const;
	void unbind() const;

	void addLayoutToBuffer(const VertexBuffer& vbo);
};

