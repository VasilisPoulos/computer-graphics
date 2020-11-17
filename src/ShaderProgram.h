#pragma once
#include "ErrorHandling.h"

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
enum class ShaderType {
	NONE = -1, 
	VERTEX = 0, 
	FRAGMENT = 1
};

/* 
 * Since we keep both shaders inside
 * the same source file, we use this
 * struct to save both sources.
 */
struct ShaderProgramSource
{
	std::string vertexShaderSource;
	std::string fragmentShaderSource;
};

class ShaderProgram
{
private:
	std::string m_Filepath;
	unsigned int m_ShaderProgramID;
	std::unordered_map<std::string, GLint> m_UniformLocationCache; // This keeps all the used uniforms
public:
	ShaderProgram(const std::string& filepath);
	~ShaderProgram();
	void bind();
	void unbind();

	GLint getAttribLocation(const GLchar* attribute);
	void setUniform4f(const std::string& uniform, GLfloat r, GLfloat g, GLfloat b, GLfloat alpha);
	void setUniform4fv(const std::string& uniform, GLsizei count, GLboolean transpose, const GLfloat* value);
	void setLayout(const GLchar* attribName, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);

private:
	ShaderProgramSource parseShaders(const std::string& filepath);
	GLuint compileShader(GLuint shaderType, const std::string& shaderSource);
	unsigned int createShaders(const std::string& vertexShader, const std::string& fragmentShader);
	GLint getUniformLocation(const std::string& uniform);
};

