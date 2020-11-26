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


void Camera::keyControl(bool* keys, GLfloat deltaTime)
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

	/*
	// Rotate around y axis
	if (keys[GLFW_KEY_A])
	{
		glm::quat q = glm::angleAxis(.01f, glm::vec3(0, 1, 0));
		front = q * front ;
	}

	if (keys[GLFW_KEY_D])
	{
		glm::quat q = glm::angleAxis(-.01f, glm::vec3(0, 1, 0));
		front = q * front ;
	}

	// Rotate around z axis
	if (keys[GLFW_KEY_X])
	{
		glm::quat q = glm::angleAxis(.01f, glm::vec3(1, 0,0));
		front = q * front ;
	}


	if (keys[GLFW_KEY_D])
	{
		glm::quat q = glm::angleAxis(-.01f, glm::vec3(1, 0, 0));
		front = q * front ;
	}
	*/
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 100.0f)
	{
		pitch = 100.0f;
	}

	if (pitch < 0.0f)
	{
		pitch = 0.0f;
	}

	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
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

Camera::~Camera()
{}
