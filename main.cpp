
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> // translate, rotate, scale, identity
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects()
{

		//6-------------/7
	  //  .           // |
	//2--------------3   |
	//    .          |   |
	//    .          |   |
	//    .          |   |
	//    .          |   |
	//    4.......   |   /5
	//               | //
	//0--------------/1


	/*
	unsigned int indices[] = {
		0,1,3,
		3,2,0,
		3,1,5,
		5,7,3,
		5,4,6,
		6,7,5,
		4,0,2,
		2,6,4,
		2,3,7,
		7,6,2,
		4,5,1,
		1,0,4
	};

	GLfloat vertices[] = {

		0.0f, 0.0f, 0.0f, // 0
		100.0f, 0.0f, 0.0f, // 1
		0.0f, 0.0f, 100.0f, // 2
		100.0f, 0.0f, 100.0f, // 3

		0.0f, 100.0f, 0.0f, // 4
		100.0f, 100.0f, 0.0f, // 5
		0.0f, 100.0f, 100.0f, // 6
		100.0f, 100.0f, 100.0f // 7
		
	};
	*/

	
	unsigned int indices[] = {
		0,1,3,
		3,2,0,
		3,1,5,
		5,7,3,
		5,4,6,
		6,7,5,
		4,0,2,
		2,6,4,
		2,3,7,
		7,6,2,
		4,5,1,
		1,0,4
	};

	GLfloat vertices[] = {
		-0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,

		-0.5f, 0.0f, 0.5f,
		0.5f, 0.0f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f
	};
	

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 24, 36);
	meshList.push_back(obj1);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow.initialize();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, -2.95f), glm::vec3(0.0f,-0.01f, 0.0f), 90.0f, 0.0f, 3.0f, 0.5f);
	//camera = Camera(glm::vec3(50.0f, 0.0f, 50.0f), glm::vec3(0.0f,1.0f, 0.0f), 90.0f, 0.0f, 5.0f, 0.5f);


	GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0 ;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until windows closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		// Get+Handle user inputs
		glfwPollEvents();

		// Camera
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		//camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		
		glm::mat4 model = glm::identity<glm::mat4>();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[0]->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}