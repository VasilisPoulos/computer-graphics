#pragma once
#include "Camera.h"
#include "Object.h"
#include "ShaderProgram.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/ext.hpp"
#include <list>


// Window Settings.
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

// Target SPH sphere to enable/disable its texture.
std::list<Object*> target;
glm::vec3 sphereControl;
float SPH_speed = 0.09;

// List of spawned objects.
std::list<Object> spawnObjects;
std::list<Object>::iterator it;

// Actual calss members for when we create one
GLFWwindow* m_window;
Object m_SPH;
Object m_SC;
ShaderProgram m_shader;
Camera m_camera;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

GLFWwindow* InitWindow();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void controlSphere(GLFWwindow* window, int key, int scancode, int action, int mods);
void controlCamera(GLFWwindow* window, int key, int scancode, int action, int mods);
void genObject();


void drawScene();
void drawSceneCube(Object& SC_cube, ShaderProgram& shaderProgram);
void drawTestCube(Object& TEST_cube_2, ShaderProgram& shaderProgram);
void drawSPH(Object& SPH_sphere, ShaderProgram& shaderProgram);

// Initializers
void initSPH(Object& SPH_sphere);
void initSC(Object& SC_cube);
void initCamera();

void startDisplaying();

void initMembers();
float randomFloat(float a, float b);
