#include "VertexArray.h"
VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_VertexArrayID));
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_VertexArrayID)); 
}

void VertexArray::bind() const { 
	GLCall(glBindVertexArray(m_VertexArrayID)); 
}

void VertexArray::unbind() const { 
	GLCall(glBindVertexArray(0)); 
}

void VertexArray::addLayoutToBuffer(const VertexBuffer& vbo) {
	bind();		// Bind the Vertex Array
	vbo.bind(); // Bind the Vertex Buffer

	// Add the layout to the buffer
	// glVertexAttribPointer(index, size, type, normalization, stide, offset);
}