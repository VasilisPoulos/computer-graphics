#pragma once

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "glm\gtc\quaternion.hpp"                          //<-- to help us rotate things

#include <GLFW\glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startMoveSpeed);

	void zoomIn(GLfloat deltaTime);
	void zoomOut(GLfloat deltaTime);
	void rotateYPositive();
	void rotateYNegative();
	void rotateXPositive();
	void rotateXNegative();

	glm::mat4 calculateViewMatrix();

	//~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();
};