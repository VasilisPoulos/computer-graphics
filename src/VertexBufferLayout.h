#pragma once
#include <GL/glew.h>

struct VertexAttribute 
{
	GLint size;			  // The number of contents
	GLenum type;		  // The type of the contents
	GLboolean normalized; // Whether the attribute should be normalized
};

class VertexBufferLayout 
{
private: 
	VertexAttribute m_Attribute;
	GLuint  m_Index;
	GLsizei m_Stride;
	GLvoid* m_offset;
public:
	VertexBufferLayout();
};

