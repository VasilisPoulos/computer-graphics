
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
#include "Object.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Shaders pack 1
static const char* vShader = "Shaders/shader.vert";
static const char* fShader = "Shaders/shader.frag";

// Shaders pack 2
static const char* vShader2 = "Shaders/shader1.vert";
static const char* fShader2 = "Shaders/shader1.frag";

//Others
GLuint VBO, VAO, shader, uniformXMove;
bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.000001f;

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
		-10.5f, 0.0f, 0.0f,
		10.5f, 0.0f, 0.0f,
		-10.5f, 10.5f, 0.0f,
		10.5f, 10.5f, 0.0f,

		-10.5f, 0.0f, 10.5f,
		10.5f, 0.0f, 10.5f,
		-10.5f, 10.5f, 10.5f,
		10.5f, 10.5f, 10.5f
	};


	unsigned int indices2[] = {
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

	GLfloat vertices2[] = {
		-0.15f, 0.0f, 0.0f,
		0.15f, 0.0f, 0.0f,
		-0.15f, 0.15f, 0.0f,
		0.15f, 0.15f, 0.0f,

		-0.15f, 0.0f, 0.15f,
		0.15f, 0.0f, 0.15f,
		-0.15f, 0.15f, 0.15f,
		0.15f, 0.15f, 0.15f
	};
	
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 24, 36);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices2, indices2, 24, 36);
	meshList.push_back(obj2);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShader2, fShader2);
	shaderList.push_back(*shader2);
}

int main()
{
	mainWindow.initialize();

	// Loading model from .obj
	Object suzi(TYPE_SUZI);
	suzi.bindVBO();

	// TODO: remove Create objects\
	make cube using the Object.cpp class
	CreateObjects();
	CreateShaders();

	// OGL parameters
	glDisable(GL_CULL_FACE);

	// Camera init
	camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, -0.1f, 0.0f), 90.0f, 0.0f, 3.0f, 0.5f);
	GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0 ;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f,100.0f); // 100

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		// Get+Handle user inputs
		glfwPollEvents();

		// Camera
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Movement
		triOffset += triIncrement;

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Main Cube
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

		glm::mat4 model = glm::identity<glm::mat4>();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[0]->RenderMesh();

		// Drawing suzi (currently testing shaders)
		suzi.bindVAO();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, suzi.texture);
		glDrawArrays(GL_TRIANGLES, 0, suzi.m_vertices.size());
		glBindTexture(GL_TEXTURE_2D, 0);
		suzi.unbindVAO();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	// TODO: cleanup after loop
	return 0;
}