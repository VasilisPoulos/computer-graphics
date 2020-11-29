#pragma once
#include "Camera.h"
#include "Object.h"
#include "ShaderProgram.h"
#include "glm/ext.hpp"
#include <list>


float randomFloat(float a, float b);
// Window Settings.
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

// TEST Camera init positions.
int move_on_x = 50;
int move_on_y = 50;
int move_on_z = 250;

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
Object m_test;
ShaderProgram m_shader;
Camera m_camera;

GLfloat deltaTime = 0;
GLfloat lastFrame = 0;

GLFWwindow* InitWindow();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void controlSphere(GLFWwindow* window, int key, int scancode, int action, int mods);
void controlCamera(GLFWwindow* window, int key, int scancode, int action, int mods);
void genObject();

void updateCamera();

void drawScene();
void drawSceneCube(Object& SC_cube, ShaderProgram& shaderProgram);
void drawTestCube(Object& TEST_cube_2, ShaderProgram& shaderProgram);
void drawSPH(Object& SPH_sphere, ShaderProgram& shaderProgram);
void accelerateSpawnables(float acceleration_value);

void initSPH(Object& SPH_sphere);
void initSC(Object& SC_cube);
void initCamera();

void startDisplaying();

void initMembers();

//	REMOVE USED FOR DEBUGGING
void initTestCube(Object& TEST_cube);
void drawTestCube(Object& TEST_cube, ShaderProgram& shaderProgram);