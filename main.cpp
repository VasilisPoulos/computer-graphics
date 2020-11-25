#include "Object.h"
#include "ShaderProgram.h"
#include "Window.h"
#include <list>

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;
int move_on_x = 15;
int move_on_y = 25;
int move_on_z = 40;
std::list<Object*> shapesList;

glm::mat4 MVP;
glm::vec3 sphereController;
std::list<Object*> target; // Target SPH sphere to enable/disable its texture

// Methods Declarations
void moveCamera(int move_x, int move_y, int move_z);

GLFWwindow* Init();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void controlCamera(GLFWwindow* window, int key, int scancode, int action, int mods);
void controlSphere(GLFWwindow* window, int key, int scancode, int action, int mods);
void updateSPH(Object ball, GLuint MatrixID, glm::mat4 Projection, glm::mat4 View, GLuint color, GLuint textureFlag, GLuint transparency);

int main()
{
	GLFWwindow * window = Init();
	ShaderProgram shaderProgram;

	// Loading cube
	Object cube(TYPE_CUBE);
	cube.scale(glm::vec3(15.0f, 15.0f, 15.0f));
	cube.randomRGB();

	// Loading cube
	Object cube_2(TYPE_CUBE);

	// Create shpere and add it to the shapes list
	Object ball(TYPE_BALL);
	shapesList.push_back(&ball);
	ball.loadTexture("res/textures/normal.jpg");

	//  Camera
	GLuint MatrixID = shaderProgram.getUniformLocation("MVP");
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);
	glm::mat4 View = glm::lookAt(
		glm::vec3(move_on_x, move_on_y, move_on_z), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Texture enable flag
	GLuint textureFlag = shaderProgram.getUniformLocation("textureFlag");
	GLuint color = shaderProgram.getUniformLocation("set_color");
	GLuint transparency = shaderProgram.getUniformLocation("transparency");
	
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.bind();

		/*
		* The order we draw objects is VERY important. To render correctly
		* transparent primitives against transparent primitives, you can't rely on
		* the z-buffer, you have to depth-sort your geometry or use a
		* depth-independant rendering technique.
		*
		* In our case we are rendering all opaque objects in the scene (letting
		* the z-buffer figure out what's visible) and then render all transparent
		* objects from back to front.
		*/

		ball.bindVAO();
		glUniform3f(color, ball.color[0], ball.color[1], ball.color[2]);
		glUniform4f(textureFlag, 1.0f, 1.0f, 1.0f, 1.0f);
		glUniform1f(transparency, 1.0f);

		// TODO: built this if into a funcition?
		if (ball.enableTexture)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ball.texture);
		}

		ball.move(sphereController);
		MVP = Projection * View * ball.modelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, ball.m_vertices.size());
		glBindTexture(GL_TEXTURE_2D, 0);
		ball.unbindVAO();

		// Draw cube behind cube to test transparency
		glUniform4f(textureFlag, 0.0f, 0.0f, 0.0f, 0.0f);
		glUniform3f(color, cube_2.color[0], cube_2.color[1], cube_2.color[2]);
		glUniform1f(transparency, 0.1f);
		cube_2.bindVAO();
		MVP = Projection * View * cube_2.modelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, cube_2.m_vertices.size());
		cube_2.unbindVAO();

		// Draw cube
		glUniform4f(textureFlag, 0.0f, 0.0f, 0.0f, 0.0f);
		glUniform3f(color, cube.color[0], cube.color[1], cube.color[2]);
		glUniform1f(transparency, 0.1f);
		cube.bindVAO();
		MVP = Projection * View * cube.modelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, cube.m_vertices.size());
		cube.unbindVAO();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
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

	// OGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glfwSetKeyCallback(window, key_callback);
	return window;
}

// Method implementations
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	else if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		target.front()->switchTexture();
	}

	controlCamera(window, key, scancode, action, mods);
	controlSphere(window, key, scancode, action, mods);
	// Sphere Controls
}

void controlCamera(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// Camera Controls
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	printf("Move Camera Left\n"); // This should be moving the ball somehow

	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
	printf("Move Camera Right\n"); // This should be moving the ball somehow

	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
	printf("Move Camera Down\n"); // This should be moving the ball somehow

	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
	printf("Move Camera Up\n"); // This should be moving the ball somehow

	else if (key == GLFW_KEY_X && action == GLFW_PRESS)
	printf("Move Camera Back\n"); // This should be moving the ball somehow

	else if (key == GLFW_KEY_E && action == GLFW_PRESS)
	printf("Move Camera Front\n"); // This should be moving the ball somehow
}

void controlSphere(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) 
		sphereController = glm::vec3(-1.0f, 0.0f, 0.0f);
		
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) 
		sphereController = glm::vec3(1.0f, 0.0f, 0.0f);

	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		sphereController = glm::vec3(0.0f, -1.0f, 0.0f);

	else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		sphereController = glm::vec3(0.0f, 1.0f, 0.0f);

	else if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS)
		sphereController = glm::vec3(0.0f, 0.0f, 1.0f);

	else if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
		sphereController = glm::vec3(0.0f, 0.0f, -1.0f);
}

void updateSPH(Object ball, GLuint MatrixID, glm::mat4 Projection, glm::mat4 View, GLuint color, GLuint textureFlag, GLuint transparency) {
	ball.bindVAO();
	glUniform3f(color, ball.color[0], ball.color[1], ball.color[2]);
	glUniform4f(textureFlag, 1.0f, 1.0f, 1.0f, 1.0f);
	glUniform1f(transparency, 1.0f);

	// TODO: built this if into a funcition?
	if (ball.enableTexture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ball.texture);
	}

	ball.move(sphereController);
	MVP = Projection * View * ball.modelMatrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, ball.m_vertices.size());
	glBindTexture(GL_TEXTURE_2D, 0);
	ball.unbindVAO();
}