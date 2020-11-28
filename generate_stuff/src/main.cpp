#include "Application.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

int main(void)
{
	initMembers();
	startDisplaying();
	return 0;
}

// Constructor
void initMembers() {
	m_window = InitWindow();
	Object SC(TYPE_CUBE);
	initSC(SC);
	m_SC = SC;

	Object SPH_sphere(TYPE_SPHERE);
	initSPH(SPH_sphere);
	m_SPH = SPH_sphere;

	ShaderProgram shader("./res/shaders/Final.shader");
	m_shader = shader;

	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 100.0f, 300.0f);
	m_shader.setUniform4fv("proj_matrix", 1, GL_FALSE, glm::value_ptr(Projection));

	glm::mat4 View = glm::lookAt(
		glm::vec3(move_on_x, move_on_y, move_on_z), // Camera is at (4,3,3), in World Space
		glm::vec3(50, 50, 50),						// and looks at the origin
		glm::vec3(0, 1, 0)							// Head is up (set to 0,-1,0 to look upside-down)
	);

	m_shader.setUniform4fv("view_matrix", 1, GL_FALSE, glm::value_ptr(View));
}

GLFWwindow* InitWindow()
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

	glfwSetKeyCallback(window, key_callback);
	return window;
}

void startDisplaying()
{
	/*
	 *	Uniform locations.
	 *	textureFlag  (vec4)	= enables/disables the texture of the object, if there
	 *						  is one.
	 *	set_color	 (vec3) = the color that an object should have when drawn.
	 *	transparency (vec1)	= the level of an objects transparency
	 */
	
	while (!glfwWindowShouldClose(m_window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_shader.bind();

		// updateCamera();
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

		drawScene();

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
	// Cleanup. 

	glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
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
	else if (key == GLFW_KEY_COMMA && action == GLFW_PRESS)
	{
		accelerateSpawnables(0.9);
	}
	else if (key == GLFW_KEY_PERIOD && action == GLFW_PRESS)
	{
		accelerateSpawnables(1.1);
	}
	controlSphere(window, key, scancode, action, mods);
}

void accelerateSpawnables(float acceleration_value)
{
	for (it = spawnObjects.begin(); it != spawnObjects.end(); ++it) {
		it->initialDirection = it->initialDirection * acceleration_value;
	}
	std::cout << "$Main :: acceleration set to " << glm::to_string(spawnObjects.begin()->initialDirection) << "\n";
} 

void genObject() {
	srand(time(NULL));
	int randomType = rand() % 3 + 1;
	Object spawnable(randomType);
	spawnable.randomRGB();

	/* Objects spawn in (0, 0, 0) this is true without the next line but
	 * it is kept form compehensiveness.
	 */
	float speed = 0.5f;
	float v_x = randomFloat(0.1, 0.9) * speed;
	float v_y = randomFloat(0.1, 0.9) * speed;
	float v_z = randomFloat(0.1, 0.9) * speed;

	glm::vec3 direction_v = glm::vec3(v_x, v_y, v_z);
	glm::vec3 translate(0.0f, 0.0f, 0.0f);
	spawnable.moveObject(translate);
	spawnable.initialDirection = direction_v;
	spawnObjects.push_back(spawnable);

	//std::cout << "44444444>"<<glm::to_string(spawnable.modelMatrix[3]) << std::endl;
	//std::cout << glm::to_string(spawnable.modelMatrix.m33) << std::endl;
	std::cout << "$Main :: Generating object of type " << randomType << " VAO ID -> " << spawnable.getVertexArrayID() << "\n";
	std::cout << "$Main :: Object type " << randomType << " ID: " << spawnable.getVertexArrayID() << " speed is (" <<
		v_x << ", " << v_y << ", " << v_z << ")\n";
}

float randomFloat(float min, float max) {
	return ((float)rand() / RAND_MAX) * (max - min) + min;
}

void controlSphere(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		sphereControl = glm::vec3(-1.0f, 0.0f, 0.0f) * SPH_speed;
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		sphereControl = glm::vec3(1.0f, 0.0f, 0.0f) * SPH_speed;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		sphereControl = glm::vec3(0.0f, -1.0f, 0.0f) * SPH_speed;
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		sphereControl = glm::vec3(0.0f, 1.0f, 0.0f) * SPH_speed;
	}
	if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS) {
		sphereControl = glm::vec3(0.0f, 0.0f, +1.0f) * SPH_speed;
	}
	if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS) {
		sphereControl = glm::vec3(0.0f, 0.0f, -1.0f) * SPH_speed;
	}
	return;
}

void initSC(Object& SC_cube)
{
	SC_cube.modelMatrix = glm::translate(SC_cube.modelMatrix, glm::vec3(50.0f, 50.0f, 50.0f));
	SC_cube.modelMatrix = glm::scale(SC_cube.modelMatrix, glm::vec3(50.0f, 50.0f, 50.0f));

	SC_cube.randomRGB();
}

void initSPH(Object& SPH_sphere)
{
	SPH_sphere.modelMatrix = glm::translate(SPH_sphere.modelMatrix, glm::vec3(50.0f, 50.0f, 50.0f));
	SPH_sphere.modelMatrix = glm::scale(SPH_sphere.modelMatrix, glm::vec3(15.0f, 15.0f, 15.0f));	// Assuming that the radius is 1

	target.push_back(&SPH_sphere);
	SPH_sphere.loadTexture("./res/textures/normal.jpg");
}

void drawScene() {
	// 1. Draw textured sphere SPH(opaque).
	drawSPH(m_SPH, m_shader);

	// 2. Spawn items (opaque).
	for (it = spawnObjects.begin(); it != spawnObjects.end(); ++it) {
		// Set up
		m_shader.setUniform4f("textureFlag", 0.0f, 0.0f, 0.0f, 0.0f);
		m_shader.setUniform3f("set_color", it->color[0], it->color[1], it->color[2]);
		m_shader.setUniform1f("transparency", 1.0f);

		// Draw
		it->bindVAO();
		m_shader.setUniform4fv("model_matrix", 1, GL_FALSE, glm::value_ptr(it->modelMatrix));
		it->detectCollision(m_SPH.modelMatrix);
		it->bounceObject(it->initialDirection);
		glDrawArrays(GL_TRIANGLES, 0, it->m_vertices.size());
		it->unbindVAO();
	}

	// 4. Draw cube (front-transparent).
	drawSceneCube(m_SC, m_shader);
}

void drawSPH(Object& SPH_sphere, ShaderProgram& shaderProgram)
{
	SPH_sphere.bindVAO();
	shaderProgram.setUniform1f("transparency", 1.0f);

	// TODO: built this if into a function?
	if (SPH_sphere.enableTexture)
	{
		shaderProgram.setUniform4f("textureFlag", 1.0f, 1.0f, 1.0f, 1.0f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, SPH_sphere.texture);
	}
	else {
		shaderProgram.setUniform4f("textureFlag", 0.0f, 0.0f, 0.0f, 0.0f);
		shaderProgram.setUniform3f("set_color", SPH_sphere.color[0], SPH_sphere.color[1], SPH_sphere.color[2]);
	}

	SPH_sphere.moveObject(sphereControl);
	shaderProgram.setUniform4fv("model_matrix", 1, GL_FALSE, glm::value_ptr(SPH_sphere.modelMatrix));
	glDrawArrays(GL_TRIANGLES, 0, SPH_sphere.m_vertices.size());
	glBindTexture(GL_TEXTURE_2D, 0);
	SPH_sphere.unbindVAO();
}

void drawSceneCube(Object& SC_cube, ShaderProgram& shaderProgram)
{
	shaderProgram.setUniform4f("textureFlag", 0.0f, 0.0f, 0.0f, 0.0f);
	shaderProgram.setUniform3f("set_color", SC_cube.color[0], SC_cube.color[1], SC_cube.color[2]);
	shaderProgram.setUniform1f("transparency", 0.1f);
	SC_cube.bindVAO();
	shaderProgram.setUniform4fv("model_matrix", 1, GL_FALSE, glm::value_ptr(SC_cube.modelMatrix));
	glDrawArrays(GL_TRIANGLES, 0, SC_cube.m_vertices.size());
	SC_cube.unbindVAO();
}
