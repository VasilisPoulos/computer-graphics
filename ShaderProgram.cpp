#include "ShaderProgram.h"
/* ShaderProgram constructor, reads from a single file both shader programs
*/
ShaderProgram::ShaderProgram(const std::string& filepath): m_Filepath(filepath), m_ShaderProgramID(0)
{
	ShaderProgramSource source = parseShaders(filepath);
	m_ShaderProgramID = createShaders(source.vertexShaderSource, source.fragmentShaderSource);
	GLCall(glUseProgram(m_ShaderProgramID));
}

/* Function reads shader programs from the provided shader source file
 *  and returns the source code devided into two sources
 */
ShaderProgramSource ShaderProgram::parseShaders(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream sources[2];

	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else {
			sources[(int)type] << line << '\n';
		}
	}

	if (sources[0].str().empty() || sources[1].str().empty())
		throw "Source File was empty!";

	return { sources[0].str(), sources[1].str() };
}

/* After reading the shaders source file, and saving both programs in a
 * specific string, now is time to create the program
 *
 */
unsigned int ShaderProgram::createShaders(const std::string& vertexShader, const std::string& fragmentShader) 
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
	GLCall(GLuint shaderID = glCreateShader(shaderType));
	const char* src = shaderSource.c_str();

	GLCall(glShaderSource(shaderID, 1, &src, nullptr));
	GLCall(glCompileShader(shaderID));

	/*Error Handling*/
	GLint status;
	GLCall(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status));
	if (status != GL_TRUE) {
		int length;														// Get the precise size of the info log
		GLCall(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length));

		char* message = (char *)_malloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(shaderID, length, &length, message));
		
		std::cout << "Failed to compile " <<
			(shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< "shader!" << std::endl;

		std::cout << message << std::endl;

		GLCall(glDeleteShader(shaderID));
		return 0;
	}
	return shaderID;
}

ShaderProgram::~ShaderProgram() 
{
	GLCall(glDeleteProgram(m_ShaderProgramID));
}

void ShaderProgram::bind()
{
	GLCall(glUseProgram(m_ShaderProgramID));
}

void ShaderProgram::unbind() 
{
	GLCall(glUseProgram(0));
}

unsigned int ShaderProgram::getID()
{
	return m_ShaderProgramID;
}

void ShaderProgram::setUniform4f(const std::string& uniform, GLfloat r, GLfloat g, GLfloat b, GLfloat alpha) 
{
	GLCall(glUniform4f(getUniformLocation(uniform), r, g, b, alpha));
}

GLint ShaderProgram::getUniformLocation(const std::string& uniform) 
{
	if(m_UniformLocationCache.find(uniform) != m_UniformLocationCache.end())
		return m_UniformLocationCache[uniform];

	GLCall(GLint location = glGetUniformLocation(m_ShaderProgramID, uniform.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform '" << uniform << "' does not exist!" << std::endl;
	m_UniformLocationCache[uniform] = location;
	return location;
}