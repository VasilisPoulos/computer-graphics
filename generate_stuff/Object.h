#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtx/transform.hpp>

const int TYPE_SUZI = 0;
const int TYPE_BALL = 1;
const int TYPE_ICOS = 2;
const int TYPE_CUBE = 3;

class Object
{
public:
	Object(int objectId);
	GLuint objectVboId;
	const char* objectPath;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint VertexArrayID;
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_uvs;
	std::vector<glm::vec3> m_normals;
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f); // Default color red
	unsigned int texture;
	bool enableTexture = false;

	bool loadOBJ(const char* path);
	void loadTexture(const char* path);
	void switchTexture();
	void bindVBO();
	void bindVAO();
	void unbindVAO();
	void randomRGB();
};
