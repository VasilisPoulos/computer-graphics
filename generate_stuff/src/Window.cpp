#include "Window.h"

Window::Window()
{
	// GLFW Init.
	if (!glfwInit())
		throw "GLFW Initialisation Failed";

	if (!createWindow())
		throw "Window Creation Failed";

	glfwMakeContextCurrent(m_displayWindow);

	glfwSetFramebufferSizeCallback(m_displayWindow, framebuffer_size_callback);

	// GLEW Init.
	if (glewInit() != GLEW_OK)
	{
		glfwDestroyWindow(m_displayWindow);
		throw "GLEW Initialisation Failed";
	}

	setOGLOptions();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Background color set to black

	//glfwSetKeyCallback(m_displayWindow, key_callback);
}

Window::~Window() {
	glfwTerminate();
}

void Window::setHints() {
	// Initialise Window.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

bool Window::createWindow() {
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Untitled Window", NULL, NULL);
	glfwSetWindowTitle(window, u8"Συγκρουόμενα");
	if (window == NULL)
		return false;

	return true;
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/*
 * 	OGL options: z-buffer and blend dont work well together
 *  be careful when rendering objects.
 */
void Window::setOGLOptions() {
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}