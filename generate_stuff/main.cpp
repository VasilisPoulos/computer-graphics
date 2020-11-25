#include "Object.h"
#include <list>

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 576;
int move_on_x = 0;
int move_on_y = 0;
int move_on_z = 10;
bool texture = true;
std::list<Object*> list;

glm::mat4 MVP;

const char* fragmentShaderCode = "#version 330 core"
"                                                                        \n"
"// Interpolated values from the vertex shaders                          \n"
"in vec2 UV;                                                             \n"
"                                                                        \n"
"// Ouput data                                                           \n"
"out vec3 color;                                                         \n"
"                                                                        \n"
"// Values that stay constant for the whole mesh.                        \n"
"uniform sampler2D myTextureSampler;                                     \n"
"                                                                        \n"
"void main() {                                                           \n"
"                                                                        \n"
"    // Output color = color of the texture at the specified UV          \n"
"    color = texture(myTextureSampler, UV).rgb;                          \n"
"}\n\0";

const char* vertexShaderCode = "#version 330 core \n"
"// Input vertex data, different for all executions of this shader.     \n"
"layout(location = 0) in vec3 vertexPosition_modelspace;                \n"
"layout(location = 1) in vec2 vertexUV;                                 \n"
"                                                                       \n"
"// Output data ; will be interpolated for each fragment.               \n"
"out vec2 UV;                                                           \n"
"                                                                       \n"
"// Values that stay constant for the whole mesh.                       \n"
"uniform mat4 MVP;                                                      \n"
"                                                                       \n"
"void main() {                                                          \n"
"                                                                       \n"
"    // Output position of the vertex, in clip space : MVP * position   \n"
"    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);            \n"
"                                                                       \n"
"    // UV of the vertex. No special space for this one.                \n"
"    UV = vertexUV;\n"
"}\n\0";

// Methods Init
void moveCamera(int move_x, int move_y, int move_z);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* Init();
GLuint createShader();
GLuint createVertexShader();
GLuint createFragmentShader();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main()
{
	// Setup 
	GLFWwindow* window = Init();
	GLuint shaderProgram = createShader();
	GLint vertexShader = createVertexShader();
	GLint fragmentShader = createFragmentShader();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
		std::cout << "Error linking program: '%s'\n", eLog;
		return 1;
	}

	// Loading cube
	Object cube(TYPE_CUBE);
	cube.bindVBO();         //bind cube's data to a vbo
	// Move cube to the side temporarilly
	cube.modelMatrix =		
		glm::translate(cube.modelMatrix, glm::vec3(2.0f, 0.0f, 0.0f));

	// Loading shpere
	Object ball(TYPE_BALL);
	list.push_back(&ball);
	ball.bindVBO();
	ball.loadTexture("jpg/magma.jpg");

	// OGL options
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5f, 0.0f, 0.0f, 1.0f);

	//  Camera
	GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 View = glm::lookAt(
		glm::vec3(move_on_x, move_on_y, move_on_z), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSetKeyCallback(window, key_callback);
		glUseProgram(shaderProgram);

		// Draw cube
		cube.bindVAO();
		MVP = Projection * View * cube.modelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, cube.m_vertices.size());
		cube.unbindVAO();

		// Draw textured sphere
		ball.bindVAO();
		// TODO: built this if into a funcition?
		if (ball.enableTexture)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ball.texture);
		}
		MVP = Projection * View * ball.modelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, ball.m_vertices.size());
		glBindTexture(GL_TEXTURE_2D, 0);
		ball.unbindVAO();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Cleanup 
	// TODO: Make a cleanup function
	glDeleteBuffers(1, &cube.vertexbuffer);
	glDeleteBuffers(1, &cube.uvbuffer);

	glDeleteBuffers(1, &ball.vertexbuffer);
	glDeleteBuffers(1, &ball.uvbuffer);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}

// Method implementations
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow* Init()
{
	// GLFW Init
	if (!glfwInit())
	{
		printf("GLFW Initialisation Failed");
		glfwTerminate();
		exit(1);
	}

	// Initialise Window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	glfwSetWindowTitle(window, u8"Συγκρουόμενα");
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window";
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// GLEW Init
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW Initialisation Failed";
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(1);
	}
	return window;
}

GLuint createShader() {
	// Shader program init
	GLuint shaderProgram = glCreateProgram();
	if (!shaderProgram)
	{
		std::cout << "Error creating shader program";
		exit(0);
	}

}

GLuint createVertexShader() {
	// Vertex Shader Object Init/Compilation
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* theCode[1];
	theCode[0] = vertexShaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(vertexShaderCode);

	glShaderSource(vertexShader, 1, theCode, codeLength);
	glCompileShader(vertexShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, sizeof(eLog), NULL, eLog);
		std::cout << "Error compiling the %d shader: '%s'\n", GL_VERTEX_SHADER, eLog;
		exit(0);
	}

	return vertexShader;
}

GLuint createFragmentShader() {

	// Fragment Shader Object Init/Compile
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* theCode[1];
	theCode[0] = fragmentShaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(fragmentShaderCode);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glShaderSource(fragmentShader, 1, theCode, codeLength);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, sizeof(eLog), NULL, eLog);
		std::cout << "Error compiling the %d shader: '%s'\n", GL_FRAGMENT_SHADER, eLog;
		exit(1);
	}

	return fragmentShader;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		move_on_y += 1;
		moveCamera(move_on_x, move_on_y, move_on_z);
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		move_on_y -= 1;
		moveCamera(move_on_x, move_on_y, move_on_z);
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		move_on_x -= 1;
		moveCamera(move_on_x, move_on_y, move_on_z);
	}
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		move_on_x += 1;
		moveCamera(move_on_x, move_on_y, move_on_z);
	}
	else if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS)
	{
		move_on_z += 1;
		moveCamera(move_on_x, move_on_y, move_on_z);
	}
	else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
	{
		move_on_z -= 1;
		moveCamera(move_on_x, move_on_y, move_on_z);
	}
	else if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{

		list.front()->switchTexture();
	}
}

void moveCamera(int move_x, int move_y, int move_z) {
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 View = glm::lookAt(
		glm::vec3(move_x, move_y, move_z), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(1, 0, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	MVP = Projection * View * Model;
	std::cout << "recorded " << move_x << ":" << move_y << ":" << move_z << "\n";
}
