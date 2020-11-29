#include "Camera.h"
#include <iostream>
#include "glm/gtc/quaternion.hpp" // Help us rotate things

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed)
{
	position = startPosition;
	worldUp = startUp;
	moveSpeed = startMoveSpeed;
}

// W
void Camera::zoomIn(GLfloat deltaTime)
{
	position -= position * 0.1f;
}

// S
void Camera::zoomOut(GLfloat deltaTime)
{
	//position -= front * 10.5f; // Zoom out
	position += position * 0.1f;
}

// D
void Camera::rotateYPositive()
{
	glm::quat q = glm::angleAxis(.10f, glm::vec3(0, 1, 0));
	position = q * position;
}

// A
void Camera::rotateYNegative()
{
	glm::quat q = glm::angleAxis(-.10f, glm::vec3(0, 1, 0));
	position = q * position;
}

// E
void Camera::rotateXPositive()
{
	glm::quat q = glm::angleAxis(.10f, glm::vec3(1, 0, 0));
	position = q * position;
}

// X
void Camera::rotateXNegative()
{
	glm::quat q = glm::angleAxis(-.10f, glm::vec3(1, 0, 0));
	position = q * position;
}

glm::mat4 Camera::calculateViewMatrix()
{
	// glm::vec3(move_on_x, move_on_y, move_on_z), // Camera is at (4,3,3), in World Space
	// glm::vec3(50.0f, 50.0f, 50.0f),			   // and looks at the origin
	// glm::vec3(0, 1, 0)
	return glm::lookAt(position, glm::vec3(50.0f, 50.0f, 50.0f), worldUp);
}
