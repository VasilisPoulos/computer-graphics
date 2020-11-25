#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>;

class Shader
{
public:
	Shader();

	void CreateFromFiles(const char* vShader, const char* fShader);
	void UseShader() { glUseProgram(this->shader); }

	GLuint GetModelLocation() { return this->uniformModel; }
	GLuint GetProjectionLocation() { return this->uniformProjection; }
	GLuint GetViewLocation() { return this->uniformView; }
	GLuint GetUniformLocation() { return this->uniformLocation; }

	~Shader();

private:
	GLuint shader,
		uniformModel,
		uniformProjection,
		uniformView,
		uniformColour,
		uniformLocation;

	std::string readShaderCodeFromFile(const char* shaderPath);

	void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	void compileShaders(const char* vShaderCode, const char* fShaderCode);
};