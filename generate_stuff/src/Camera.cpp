#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}


void Camera::keyControl(bool* keys, GLfloat deltaTime,GLfloat xChange,GLfloat yChange)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += front * velocity; // Zoom in
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity; // Zoom out
	}


	// Rotate around y axis
	if (keys[GLFW_KEY_A])
	{
		//position -= right * velocity;
		const float radius = 10.0f;
		position.x = cos(glfwGetTime()) * radius;
		//position.z = cos(glfwGetTime()) * radius;
		//update();
	}

	if (keys[GLFW_KEY_D])
	{
		float radius = 10.0f;
		//position.x = sin(glfwGetTime()) * radius;
		position.z = sin(glfwGetTime()) * radius;
	}

	// Rotate around y axis
	if (keys[GLFW_KEY_E])
	{
		//position -= right * velocity;
		const float radius = 10.0f;
		position.y = sin(glfwGetTime()) * radius;
	}

	if (keys[GLFW_KEY_X])
	{
		float radius = 10.0f;
		position.z = cos(glfwGetTime()) * radius;;
	}
	
}


glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, glm::vec3(0.0, 0.0, 0.0), up);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera(){}
