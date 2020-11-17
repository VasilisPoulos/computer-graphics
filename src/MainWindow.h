#pragma once
#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "ErrorHandling.h"
#include "VertexArray.h"

// Window Dimensions
const GLint WIDTH = 600, HEIGHT = 600;

class MainWindow
{
private:
	GLFWwindow* m_Window;
public:
	MainWindow();
	~MainWindow();
	void display();
};

