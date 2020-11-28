#pragma once

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

//#include "glm\gtc\quaternion.hpp"                          //<-- to help us rotate things

#include <GLFW\glfw3.h>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);

	glm::mat4 calculateViewMatrix(char x);

	glm::mat4 calculateViewMatrix();

	//~Camera();

	void zoomIn(GLfloat deltaTime);
	void zoomOut(GLfloat deltaTime);

	void rotateYPositive();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	//glm::vec3 target(50.0f, 50.0f, 50.0f);

	void update();
};