#pragma once
#include <GL/glew.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexShaderSource;
	std::string FragmentShaderSource;
};

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	void bind()	const { glUseProgram(m_ProgramID); }
	void unbind() const { glUseProgram(0); }

	void setUniform4f(const std::string& uniform, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	void setUniform4fv(const std::string& uniform, GLsizei count, GLboolean transpose, const GLfloat* value);
	GLint getAttribLocation(const GLchar* attribute) { return glGetAttribLocation(m_ProgramID, attribute);}
	GLint getUniformLocation(const std::string& uniform);
private:
	const char* shaderPath = "shader/shader.glsl";
	GLuint m_ProgramID;
	std::unordered_map<std::string, GLint> m_UniformLocationCache;

	ShaderProgramSource parseShaders(const std::string& filepath);
	GLuint createShaders(const std::string& vertexShader, const std::string& fragmentShader);
	GLuint compileShader(GLuint shaderType, const std::string& shaderSource);
};