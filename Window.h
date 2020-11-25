#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Window
{
private:
	const int WINDOW_WIDTH = 600;
	const int WINDOW_HEIGHT = 600;

	glm::mat4 projection_matrix;
	GLFWwindow* m_displayWindow;

	void setHints();
	bool createWindow();
	void setOGL();
	static void processInput(GLFWwindow* window, int key, int code, int action, int mode);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void clearScreen();
public:
	Window();
	~Window();

	void display();
};


