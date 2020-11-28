#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include "Input.h"

class Window
{
public:
	Window();
	~Window();

private:
	const int SCR_WIDTH  = 600;
	const int SCR_HEIGHT = 600;

	GLFWwindow* m_displayWindow;

	void setHints();
	bool createWindow();
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void setOGLOptions();
};