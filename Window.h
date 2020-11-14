#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>

#include "ShaderProgram.h"
#include "VertexBuffer.h"

#define WIDTH  600
#define HEIGHT 600
#define MAJOR_VERSION 3
#define MINOR_VERSION 3

class Window {
private:
	GLFWwindow* m_Window;
public:
	Window();
	~Window();
	void draw();
private:
	void catchKey();
};
