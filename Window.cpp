#include "Window.h"

#include "Window.h"

Window::Window()
{
	if (!glfwInit()) throw "GLFW library load failed...";

	setHints();

	if (!createWindow()) throw "Failed to create GLFW window";

	glfwSetFramebufferSizeCallback(m_displayWindow, framebuffer_size_callback);
	glfwSetKeyCallback(m_displayWindow, processInput);

	setOGL();
}

bool Window::createWindow() {
	m_displayWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Untitled Window", NULL, NULL);
	if (!m_displayWindow)
		return false;
	glfwSetWindowTitle(m_displayWindow, u8"Συγκρουόμενα");

	glfwMakeContextCurrent(m_displayWindow);
	return true;
}

void Window::clearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::setHints() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

Window::~Window()
{
	glfwDestroyWindow(m_displayWindow);
	glfwTerminate();
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Window::processInput(GLFWwindow* window, int key, int code, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Camera Controls
	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
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

	// Sphere Controls
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		printf("Move Sphere Left\n"); // This should be moving the ball somehow

	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		printf("Move Sphere Right\n"); // This should be moving the ball somehow

	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		printf("Move Sphere Down\n"); // This should be moving the ball somehow

	else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		printf("Move Sphere Up\n"); // This should be moving the ball somehow

	else if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS)
		printf("Move Sphere Front\n"); // This should be moving the ball somehow

	else if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
		printf("Move Sphere Back\n"); // This should be moving the ball somehow
}

void Window::setOGL() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Window::display() {
	while (!glfwWindowShouldClose(m_displayWindow))
	{
		clearScreen();

		glfwSwapBuffers(m_displayWindow);
		glfwPollEvents();
	}
}