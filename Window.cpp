#include "Window.h"

Window::Window()
{
	// Initialise GLFW
	if (!glfwInit()) throw "Failed to initialize GLFW.";

	// Set Hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_VERSION);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Open a window and create its OpenGL context
	m_Window = glfwCreateWindow(WIDTH, HEIGHT, "Untitled Window", NULL, NULL);
	if (!m_Window) throw "Failed to open GLFW window.";
	glfwSetWindowTitle(m_Window, u8"Συγκρουόμενα");
	glfwMakeContextCurrent(m_Window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) throw "Failed to initialize GLEW.";

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GL_TRUE);

	// Black background (never go back to other backgrounds)
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Window::draw() {
	// Create the Vertex Array Object before linking
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLfloat vertices[]{
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
	};
	VertexBuffer vbo(vertices, sizeof(vertices), GL_STATIC_DRAW);

	GLuint elements[]{
		0, 1, 2,
		2, 3, 0
	};

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	ShaderProgram shader("./res/shaders/Basic.shader");

	// Set position attribute layout
	GLCall(GLint posAttrib = glGetAttribLocation(shader.getID(), "position"));
	GLCall(glEnableVertexAttribArray(posAttrib));
	GLCall(glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0));

	// Set color attribute layout
	GLCall(GLint colAttrib = glGetAttribLocation(shader.getID(), "color"));
	GLCall(glEnableVertexAttribArray(colAttrib));
	GLCall(glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float))));
	
	/* Loop until the user closes the window */
	do 
	{		
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		/* Swap front and back buffers */
		glfwSwapBuffers(m_Window);

		/* Poll for and process events */
		glfwPollEvents();

	} while (!glfwWindowShouldClose(m_Window));
}

void Window::catchKey() {
	if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS) {
		std::cout << "Pressed key A" << std::endl;
	}

	if (glfwGetKey(m_Window, GLFW_KEY_B)) {
		std::cout << "Pressed key B" << std::endl;
	}
}

Window::~Window()
{
	glfwTerminate();
}

