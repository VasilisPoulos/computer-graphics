#include "Object.h"
#include "ShaderProgram.h"
#include <list>
#include "Camera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"

// Window Settings.
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

// TEST Camera init positions.
int move_on_x = 50;
int move_on_y = 50;
int move_on_z = 250;

// Target SPH sphere to enable/disable its texture.
std::list<Object*> target;

glm::vec3 sphereControl;

// List of spawned objects.
std::list<Object> spawnObjects;
std::list<Object>::iterator it;

// Used for MVP uniform in shaders.
glm::mat4 MVP;

// Methods Declarations.
GLFWwindow* Init();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void controlSphere(GLFWwindow* window, int key, int scancode, int action, int mods);
void genObject();
float randomFloat(float a, float b);

int main()
{
	GLFWwindow* window = Init();
	ShaderProgram shaderProgram;

	Object SC_cube(TYPE_CUBE);
	SC_cube.modelMatrix = glm::translate(SC_cube.modelMatrix, glm::vec3(50.0f, 50.0f, 50.0f));
	SC_cube.modelMatrix = glm::scale(SC_cube.modelMatrix, glm::vec3(50.0f, 50.0f, 50.0f));
	
	SC_cube.randomRGB();

	// Test cube for debuging.
	Object TEST_cube_2(TYPE_CUBE);

	Object SPH_sphere(TYPE_SPHERE);
	SPH_sphere.modelMatrix = glm::translate(SPH_sphere.modelMatrix, glm::vec3(50.0f, 50.0f, 50.0f));
	SPH_sphere.modelMatrix = glm::scale(SPH_sphere.modelMatrix, glm::vec3(15.0f, 15.0f, 15.0f));	// Assuming that the radius is 1
	
	target.push_back(&SPH_sphere);
	SPH_sphere.loadTexture("./res/textures/normal.jpg");

	/*
	 *	TEST Camera init.
	 */
	Camera camera;
	camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, -0.1f, 0.0f), 90.0f, 0.0f, 3.0f, 0.5f);
	GLuint MatrixID = shaderProgram.getUniformLocation("MVP");
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 100.0f, 300.0f);
	glm::mat4 View = glm::lookAt(
		glm::vec3(move_on_x, move_on_y, move_on_z), // Camera is at (4,3,3), in World Space
		glm::vec3(50, 50, 50),						// and looks at the origin
		glm::vec3(0, 1, 0)							// Head is up (set to 0,-1,0 to look upside-down)
	);

	/*
	 *	Uniform locations.
	 *	textureFlag  (vec4)	= enables/disables the texture of the object, if there
	 *						  is one.
	 *	color		 (vec3) = the color that an object should have when drawn.
	 *	transparency (vec1)	= the level of an objects transparency
	 */
	GLuint textureFlag = shaderProgram.getUniformLocation("textureFlag");
	GLuint color = shaderProgram.getUniformLocation("set_color");
	GLuint transparency = shaderProgram.getUniformLocation("transparency");

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSetKeyCallback(window, key_callback);

		shaderProgram.bind();
		//camera.keyControl();

		/*
		* The order we draw objects is VERY important. To render correctly
		* transparent primitives against transparent primitives, you can't rely
		* on the z-buffer, you have to depth-sort your geometry or use a
		* depth-independant rendering technique.
		*
		* In our case we are rendering all opaque objects in the scene (letting
		* the z-buffer figure out what's visible) and then render all transparent
		* objects from back to front.
		*/

		// 1. Draw textured sphere SPH(opaque).
		SPH_sphere.bindVAO();
		glUniform1f(transparency, 1.0f);
		// TODO: built this if into a funcition?
		if (SPH_sphere.enableTexture)
		{
			glUniform4f(textureFlag, 1.0f, 1.0f, 1.0f, 1.0f);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, SPH_sphere.texture);
		}
		else {
			glUniform4f(textureFlag, 0.0f, 0.0f, 0.0f, 0.0f);
			glUniform3f(color, SPH_sphere.color[0], SPH_sphere.color[1], SPH_sphere.color[2]);
		}
		SPH_sphere.moveObject(sphereControl);
		MVP = Projection * View * SPH_sphere.modelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, SPH_sphere.m_vertices.size());
		glBindTexture(GL_TEXTURE_2D, 0);
		SPH_sphere.unbindVAO();

		// 2. Spawn items (opaque).
		for (it = spawnObjects.begin(); it != spawnObjects.end(); ++it) {
			glUniform4f(textureFlag, 0.0f, 0.0f, 0.0f, 0.0f);
			glUniform3f(color, it->color[0], it->color[1], it->color[2]);
			glUniform1f(transparency, 1.0f);
			it->bindVAO();
			//it->moveFrom(glm::to_string(it->modelMatrix[3]).c_str());
			MVP = Projection * View * it->modelMatrix;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, it->m_vertices.size());
			it->unbindVAO();
		}

		// 3. Draw cube behind cube to TEST transparency (back-transparent).
		glUniform4f(textureFlag, 0.0f, 0.0f, 0.0f, 0.0f);
		glUniform3f(color, TEST_cube_2.color[0], TEST_cube_2.color[1], TEST_cube_2.color[2]);
		glUniform1f(transparency, 0.1f);
		TEST_cube_2.bindVAO();
		MVP = Projection * View * TEST_cube_2.modelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, TEST_cube_2.m_vertices.size());
		TEST_cube_2.unbindVAO();

		// 4. Draw cube (front-transparent).
		glUniform4f(textureFlag, 0.0f, 0.0f, 0.0f, 0.0f);
		glUniform3f(color, SC_cube.color[0], SC_cube.color[1], SC_cube.color[2]);
		glUniform1f(transparency, 0.1f);
		SC_cube.bindVAO();
		MVP = Projection * View * SC_cube.modelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, SC_cube.m_vertices.size());
		SC_cube.unbindVAO();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Cleanup. 

	glfwTerminate();
	return 0;
}

// Method implementations.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow* Init()
{
	// GLFW Init.
	if (!glfwInit())
	{
		printf("GLFW Initialisation Failed");
		glfwTerminate();
		exit(1);
	}

	// Initialise Window.
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

	// GLEW Init.
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW Initialisation Failed";
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(1);
	}

	/*
	 * 	OGL options: z-buffer and blend dont work well togeather
	 *  be careful when rendering objects.
	 */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	return window;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		target.front()->switchTexture();
	}
	else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		genObject();	
	}
	controlSphere(window, key, scancode, action, mods);
}

float randomFloat(float min, float max) {
	return ((float)rand() / RAND_MAX) * (max - min) + min;
}

void genObject() {
	srand(time(NULL));
	int randomType = rand() % 3 + 1;
	Object spawnable(randomType);
	spawnable.randomRGB();

	/* Objects spawn in (0, 0, 0) this is true without the next line but
	 * it is kept form compehensiveness.
	 */
	
	float v_x = randomFloat(0.1, 0.9);
	float v_y = randomFloat(0.1, 0.9);
	float v_z = randomFloat(0.1, 0.9);

	glm::vec3 direction_v = glm::vec3(v_x, v_y, v_z);
	glm::vec3 translate(0.0f, 0.0f, 0.0f);
	spawnable.moveObject(translate);
	spawnObjects.push_back(spawnable);

	//std::cout << "44444444>"<<glm::to_string(spawnable.modelMatrix[3]) << std::endl;
	//std::cout << glm::to_string(spawnable.modelMatrix.m33) << std::endl;
	std::cout << "$Main :: Generating object of type " << randomType << " VAO ID -> " << spawnable.getVertexArrayID() << "\n";
	std::cout << "$Main :: Object type "<< randomType << " ID: " << spawnable.getVertexArrayID() << " speed is ("<< 
		v_x <<", "<< v_y << ", " << v_z <<")\n";
}

void controlSphere(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		sphereControl = glm::vec3(-1.0f, 0.0f, 0.0f);
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		sphereControl = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		sphereControl = glm::vec3(0.0f, -1.0f, 0.0f);
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		sphereControl = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS) {
		sphereControl = glm::vec3(0.0f, 0.0f, +1.0f);
	}
	if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS) {
		sphereControl = glm::vec3(0.0f, 0.0f, -1.0f);
	}
	return;
}
