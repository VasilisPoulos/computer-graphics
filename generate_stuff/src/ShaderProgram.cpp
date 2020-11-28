#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() {}

ShaderProgram::ShaderProgram(const std::string& filepath): m_ProgramID(0)
{
	ShaderProgramSource source = parseShaders(filepath); // Change the path to the wanted shader
	m_ProgramID = createShaders(source.VertexShaderSource, source.FragmentShaderSource);
	bind();
}

/* Function reads shader programs from the provided shader source file
 *  and returns the source code devided into two sources
 */
ShaderProgramSource ShaderProgram::parseShaders(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = 0, VERTEX = 1, FRAGMENT = 2
	};

	std::string line;
	std::stringstream sources[3];

	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) 
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;

			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else 
		{
			sources[(int)type] << line << '\n';
		}
	}

	if (sources[1].str().empty() || sources[2].str().empty())
		throw "Source File was empty!";

	return { sources[1].str(), sources[2].str() };
}

/* After reading the shaders source file, and saving both programs in a
 * specific string, now is time to create the program
 */
GLuint ShaderProgram::createShaders(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLuint program = glCreateProgram();
	GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDetachShader(program, vs);
	glDetachShader(program, fs);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

/* Provide the shader type and its source code, the function
 * creates a shader object.
 */
GLuint ShaderProgram::compileShader(GLuint shaderType, const std::string& shaderSource)
{
	GLuint shaderID = glCreateShader(shaderType);
	const char* src = shaderSource.c_str();

	glShaderSource(shaderID, 1, &src, nullptr);
	glCompileShader(shaderID);

	/*Error Handling*/
	GLint status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		int length;														// Get the precice size of the info log
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);

		char* message = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(shaderID, length, &length, message);

		std::cout << "Failed to compile " <<
			(shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< "shader!" << std::endl;

		std::cout << message << std::endl;

		glDeleteShader(shaderID);
		return 0;
	}
	return shaderID;
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_ProgramID);
}


// Set Uniforms Section
void ShaderProgram::setUniform1f(const std::string& uniform, GLfloat v0) {
	glUniform1f(getUniformLocation(uniform), v0);
}

void ShaderProgram::setUniform2f(const std::string& uniform, GLfloat v0, GLfloat v1) {
	glUniform2f(getUniformLocation(uniform), v0, v1);
}

void ShaderProgram::setUniform3f(const std::string& uniform, GLfloat v0, GLfloat v1, GLfloat v2) {
	glUniform3f(getUniformLocation(uniform), v0, v1, v2);
}

void ShaderProgram::setUniform4f(const std::string& uniform, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	glUniform4f(getUniformLocation(uniform), v0, v1, v2, v3);
}

void ShaderProgram::setUniform4fv(const std::string& uniform, GLsizei count, GLboolean transpose, const GLfloat* value) {
	glUniformMatrix4fv(getUniformLocation(uniform), count, transpose, value);
}

GLint ShaderProgram::getUniformLocation(const std::string& uniform)
{
	if (m_UniformLocationCache.find(uniform) != m_UniformLocationCache.end())
		return m_UniformLocationCache[uniform];

	GLint location = glGetUniformLocation(m_ProgramID, uniform.c_str());
	if (location == -1)
		std::cout << "Warning: uniform '" << uniform << "' does not exist!" << std::endl;

	m_UniformLocationCache[uniform] = location;
	return location;
}