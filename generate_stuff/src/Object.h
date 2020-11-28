#include <vector>
#include <iostream>

#include "Utilities.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtx/transform.hpp>

const int TYPE_SUZI = 0;
const int TYPE_SPHERE = 1;
const int TYPE_CILINDER = 2;
const int TYPE_CUBE = 3;

class Object
{
public:
	//TODO: Make some private, clean up
	Object();
	Object(int objectId);
	//~Object();
	const char* objectPath;

	// Geometric parameters.
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_uvs;
	std::vector<glm::vec3> m_normals;
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::vec3 initialDirection = glm::vec3(0.0f, 0.0f, 0.0f);

	// Texture and color.
	glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f); // Default color red
	unsigned int texture;
	bool enableTexture = false;
	bool isSpawnable = false;

	void loadTexture(const char* path);
	void switchTexture();
	void bindVAO();
	void unbindVAO();
	void randomRGB();
	void moveFrom(char* input);
	void bounceObject(glm::vec3 direction);
	void moveObject(glm::vec3& translation);
	void detectCollision(glm::mat4 shpereMatrix);
	GLuint getVertexArrayID();

private:
	GLuint vertexArrayID;
	GLuint vertexBufferID;
	GLuint vertexbuffer;
	GLuint uvbuffer;

	void bindVBO();
	bool loadOBJ(const char* path);
};
