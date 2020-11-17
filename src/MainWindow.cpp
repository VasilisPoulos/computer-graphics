#include "MainWindow.h"

MainWindow::MainWindow() {
	// Initialise GLFW
	if (!glfwInit()) throw "GLFW initialization failed!";

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Core profile = No backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Allow forward compability
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	m_Window = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);

	if (!m_Window) throw "GLFW window creation failed!";

	// Get Buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(m_Window, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(m_Window);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
		throw "GLEW initialisation failed!";

	//glEnable(GL_CULL_FACE);
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glBlendEquation(GL_FUNC_ADD));
	//glCullFace(GL_FRONT);

	//glEnable(GL_CULL_FACE);
	GLCall(glFrontFace(GL_CCW));

	// Create Viewport
	GLCall(glViewport(0, 0, bufferWidth, bufferHeight));

	glfwSwapInterval(1);
}

MainWindow::~MainWindow() {
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void MainWindow::display() {
	ShaderProgram shader("./res/shaders/TeoShader.shader");

	// Try it --> http://learnwebgl.brown37.net/08_projections/projections_perspective.html
	float cameraX = 020.0f;
	float cameraY = 000.0f;
	float cameraZ = 350.0f;

	float cubeLocX =  000.0f;
	float cubeLocY = -130.0f;
	float cubeLocZ =  000.0f; // Shift down Y to reveal perspective

	GLfloat cube[] = {
		// Front Face
		000.0f, 000.0f, 000.0f,
		000.0f, 000.0f, 100.0f,
		100.0f, 000.0f, 000.0f,

		000.0f, 000.0f, 100.0f,
		100.0f, 000.0f, 000.0f,
		100.0f, 000.0f, 100.0f,

		// Back Face
		000.0f, 100.0f, 100.0f,
		100.0f, 100.0f, 100.0f,
		100.0f, 100.0f, 000.0f,

		100.0f, 100.0f, 000.0f,
		000.0f, 100.0f, 000.0f,
		000.0f, 100.0f, 100.0f,

		// Righ Face
		100.0f, 000.0f, 100.0f,
		100.0f, 100.0f, 000.0f,
		100.0f, 100.0f, 100.0f,

		100.0f, 000.0f, 100.0f,
		100.0f, 100.0f, 000.0f,
		100.0f, 000.0f, 000.0f,

		// Left Face
		000.0f, 000.0f, 100.0f,
		000.0f, 100.0f, 000.0f,
		000.0f, 100.0f, 100.0f,

		000.0f, 000.0f, 100.0f,
		000.0f, 100.0f, 000.0f,
		000.0f, 000.0f, 000.0f,

		// Bottom Face
		000.0f, 100.0f, 000.0f,
		100.0f, 000.0f, 000.0f,
		100.0f, 100.0f, 000.0f,

		000.0f, 100.0f, 000.0f,
		100.0f, 000.0f, 000.0f,
		000.0f, 000.0f, 000.0f,

		// Up Face
		000.0f, 100.0f, 100.0f,
		100.0f, 100.0f, 100.0f,
		100.0f, 000.0f, 100.0f,

		000.0f, 000.0f, 100.0f,
		100.0f, 000.0f, 100.0f,
		000.0f, 100.0f, 100.0f
	};

	VertexArray vao;
	vao.bind();

	VertexBuffer vbo(cube, sizeof(cube), GL_STATIC_DRAW);

	// associate VBO with the corresponding vertex attribute in the vertex shader
	GLint index = shader.getAttribLocation("position");
	GLCall(glVertexAttribPointer(
		index, // The index of the attribute
		3, GL_FLOAT, GL_FALSE, // Attribute
		0, 0)	// The stride and offset of the attribute
	);
	GLCall(glEnableVertexAttribArray(index));

	// Allocate variables used in display() function, so that they won't need to be allocated during rendering
	GLuint mvLoc, projLoc;
	float aspect;
	glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;

	// Loop until window closed
	do {
		GLCall(glClear(GL_DEPTH_BUFFER_BIT));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		shader.bind();

		// build perspective matrix
		aspect = (float)WIDTH / (float)HEIGHT;
		pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

		// build view matrix, model matrix, and model-view matrix
		vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
		mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));

		mvMat = vMat * mMat;

		shader.setUniform4fv("mv_matrix", 1, GL_FALSE, glm::value_ptr(mvMat));
		shader.setUniform4fv("proj_matrix", 1, GL_FALSE, glm::value_ptr(pMat));

		// adjust OpenGL settings and draw model
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LEQUAL));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 12 * 3));

		glfwSwapBuffers(m_Window);

		// Get + Handle user input events
		glfwPollEvents();
	} while (!glfwWindowShouldClose(m_Window));

	return;
}