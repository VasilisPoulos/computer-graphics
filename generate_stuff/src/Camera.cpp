#include "Camera.h"
#include <iostream>

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, 1.0f);

	moveSpeed = startMoveSpeed;
	//update();
}


void Camera::zoomIn(GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;
	position.z -= 10;
}

void Camera::zoomOut(GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;
	std::cout << velocity << std::endl;
	position.z += front.z * 5;
}

void Camera::rotateYPositive()
{
	const float radius = 10.0f;
	position.x += sin(glfwGetTime()) * radius;
	position.z += cos(glfwGetTime()) * radius;

}

glm::mat4 Camera::calculateViewMatrix()
{
	// glm::vec3(move_on_x, move_on_y, move_on_z), // Camera is at (4,3,3), in World Space
	// glm::vec3(50.0f, 50.0f, 50.0f),			   // and looks at the origin
	// glm::vec3(0, 1, 0)
	return glm::lookAt(position, glm::vec3(50.0f, 50.0f, 50.0f), worldUp);
}

void Camera::update()
{
	//front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	//front.y = sin(glm::radians(pitch));
	//front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	//front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

//Camera::~Camera(){}
