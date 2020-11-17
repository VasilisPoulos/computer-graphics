#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>


// Window Dimensions
const GLint WIDTH = 600, HEIGHT = 600;

// Alllocate variables used in display() function, so that they won't need to be allocated during rendering
GLuint mvLoc, projLoc;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;

/*
	Usually we have multiple VAO and VBO for each object
	VAO = Vertex Array Object
	VBO = Vertex Buffer Object
*/

GLuint vertexArrayID, vertexBuffer, shader;

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;

// Vertex Shader
static const char* vShader = "												\n\
#version 430																\n\
																			\n\
layout(location = 0) in vec3 position;										\n\
																			\n\
uniform mat4 mv_matrix;														\n\
uniform mat4 proj_matrix;													\n\
																			\n\
out vec4 varyingColor;														\n\
void main()																	\n\
{																			\n\
	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);				\n\
	varyingColor = vec4(1.0,0.0,0.0,0.5) ;									\n\
	//varyingColor = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.3);	\n\
}";

// Fragment Shader
static const char* fShader = "						\n\
#version 430										\n\
													\n\
in vec4 varyingColor;								\n\
													\n\
out vec4 color;										\n\
uniform mat4 mv_matrix;								\n\
uniform mat4 proj_matrix;							\n\
													\n\
void main()											\n\
{													\n\
	color = varyingColor;							\n\
	//color = vec4(varyingColor,1.0f);				\n\
}";

void createCube()
{
	GLfloat vertices[] = {
		// Front Face
		0.0f,0.0f,0.0f,
		0.0f,0.0f, 100.0f,
		100.0f, 0.0f, 0.0f,

		0.0f, 0.0f,100.0f,
		100.0f,0.0f,0.0f,
		100.0f, 0.0f,100.0f,

		// Back Face
		0.0f,100.0f, 100.0f,
		100.0f,100.0f,100.0f,
		100.0f,100.0f,0.0f,

		100.0f, 100.0f,0.0f,
		0.0f,100.0f,0.0f,
		0.0f,100.0f,100.0f,

		// Righ Face
		100.0f,0.0f,100.0f,
		100.0f, 100.0f, 0.0f,
		100.0f, 100.0f,100.0f,

		100.0f,0.0f,100.0f,
		100.0f, 100.0f, 0.0f,
		100.0f,0.0f,0.0f,

		// Left Face
		0.0f, 0.0f, 100.0f,
		0.0f,100.0f, 0.0f,
		0.0f,100.0f, 100.0f,

		0.0f, 0.0f, 100.0f,
		0.0f,100.0f, 0.0f,
		0.0f, 0.0f, 0.0f,

		// Bottom Face
		0.0f,100.0f,0.0f,
		100.0f, 0.0f, 0.0f,
		100.0f,100.0f, 0.0f,

		0.0f,100.0f,0.0f,
		100.0f, 0.0f, 0.0f,
		0.0f, 0.0f,0.0f,

		// Up Face
		0.0f, 100.0f, 100.0f,
		100.0f, 100.0f,100.0f,
		100.0f, 0.0f, 100.0f,

		0.0f, 0.0f, 100.0f,
		100.0f, 0.0f, 100.0f,
		0.0f, 100.0f, 100.0f
	};

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	// Create Buffer Object
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {

	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: %s \n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void compileShaders() {
	shader = glCreateProgram();

	if (!shader) {
		printf("Error creating shader program!\n");
		return;
	}

	addShader(shader, vShader, GL_VERTEX_SHADER);
	addShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: %s \n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: %s \n", eLog);
		return;
	}

	glValidateProgram(shader);
}

void init(GLFWwindow* window) {
	compileShaders();

	// Try it --> http://learnwebgl.brown37.net/08_projections/projections_perspective.html
	cameraX = 20.0f; cameraY = 0.0f; cameraZ = 350.0f;
	cubeLocX = 0.0f, cubeLocY = -130.0f; cubeLocZ = 0.0f; // Shift down Y to reveal perspective
	createCube();
}

void display(GLFWwindow* window, double currentTime) {

	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shader);

	// get the uniform variables for the MV and projection matrices
	mvLoc = glGetUniformLocation(shader, "mv_matrix");
	projLoc = glGetUniformLocation(shader, "proj_matrix");

	// build perspective matrix
	aspect = (float)WIDTH / (float)HEIGHT;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

	// build view matrix, model matrix, and model-view matrix
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));


	mvMat = vMat * mMat;


	// copy perspective and MV matrices to corresponding uniform variables
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// adjust OpenGL settings and draw model
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);


}

int main(void) {

	// Initialise GLFW
	if (!glfwInit()) {
		printf("GLFW initialization failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Core profile = No backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Allow forward compability
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);

	if (!mainWindow) {
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	//glCullFace(GL_FRONT);

	//glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	// Create Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSwapInterval(1);
	init(mainWindow);

	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow)) {

		display(mainWindow, glfwGetTime());
		glfwSwapBuffers(mainWindow);

		// Get + Handle user input events
		glfwPollEvents();
	}

	glfwDestroyWindow(mainWindow);
	glfwTerminate();

	return 0;
}
