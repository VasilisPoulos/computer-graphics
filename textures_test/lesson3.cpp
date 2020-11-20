#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtx/transform.hpp>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 576;
int move_on_x = 0;
int move_on_y = 0;
int move_on_z = 3;

glm::mat4 MVP;

const char* vertexShaderCode = "#version 330 core   \n"
"layout(location = 0) in vec3 aPos;                 \n"
"layout(location = 1) in vec3 aColor;               \n"
"layout(location = 2) in vec2 aTexCoord;            \n"
"                                                   \n"
"uniform mat4 MVP;                                  \n"
"                                                   \n"
"out vec3 ourColor;                                 \n"
"out vec2 TexCoord;                                 \n"
"                                                   \n"
"void main()                                        \n"
"{                                                  \n"
"    gl_Position = MVP * vec4(aPos, 1.0);           \n"
"    ourColor = aColor;                             \n"
"    TexCoord = aTexCoord;                          \n"
"}\n\0";

const char* fragmentShaderCode = "#version 330 core     \n"
"out vec4 FragColor;                                    \n"
"                                                       \n"
"in vec3 ourColor;                                      \n"
"in vec2 TexCoord;                                      \n"
"                                                       \n"
"uniform sampler2D ourTexture;                          \n"
"                                                       \n"
"void main()                                            \n"
"{                                                      \n"
"    FragColor = texture(ourTexture, TexCoord);         \n"
"}\n\0";

float cube[] = {
    -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 

     1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

     1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
     1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

     1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

    -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

     1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
    -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

    -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

     1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
     1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

     1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
     1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

     1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
     1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

     1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

     1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f
};
// Methods Init
void moveCamera(int move_x, int move_y, int move_z);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
unsigned int loadTexture();
GLFWwindow* Init();
GLuint createShader();
GLuint createVertexShader();
GLuint createFragmentShader();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main()
{
    // Setup 
    GLFWwindow* window = Init();
    GLuint shaderProgram = createShader();
    GLint vertexShader = createVertexShader();
    GLint fragmentShader = createFragmentShader();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
        std::cout << "Error linking program: '%s'\n", eLog;
        return 1;
    }

    // unsigned int VBO, VAO, EBO;
    unsigned int VBO_1, VAO_1, VBO_2, VAO_2;

    // First triangle
    glGenVertexArrays(1, &VAO_1);
    glGenBuffers(1, &VBO_1);
    glBindVertexArray(VAO_1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //  camera
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 View = glm::lookAt(
        glm::vec3(move_on_x, move_on_y, move_on_z), // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    glm::mat4 Model = glm::mat4(1.0f);
    MVP = Projection * View * Model;

    // texture work
    unsigned int texture1 = loadTexture();
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode
    // Window loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glfwSetKeyCallback(window, key_callback);

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        // Shaders.use and draw
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO_1);
        glDrawArrays(GL_TRIANGLES, 0, 12*3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Cleanup
    glDeleteVertexArrays(1, &VAO_1);
    glDeleteBuffers(1, &VBO_1);
    glDeleteVertexArrays(1, &VAO_2);
    glDeleteBuffers(1, &VBO_2);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}

// Method implementations
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLFWwindow* Init()
{
    // GLFW Init
    if (!glfwInit())
    {
        printf("GLFW Initialisation Failed");
        glfwTerminate();
        exit(1);
    }

    // Initialise Window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    glfwSetWindowTitle(window, u8"Συγκρουόμενα");
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window";
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLEW Init
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW Initialisation Failed";
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(1);
    }
    return window;
}

GLuint createShader() {
    // Shader program init
    GLuint shaderProgram = glCreateProgram();
    if (!shaderProgram)
    {
        std::cout << "Error creating shader program";
        exit(0);
    }

}

GLuint createVertexShader() {
    // Vertex Shader Object Init/Compilation
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* theCode[1];
    theCode[0] = vertexShaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(vertexShaderCode);

    glShaderSource(vertexShader, 1, theCode, codeLength);
    glCompileShader(vertexShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(vertexShader, sizeof(eLog), NULL, eLog);
        std::cout << "Error compiling the %d shader: '%s'\n", GL_VERTEX_SHADER, eLog;
        exit(0);
    }

    return vertexShader;
}

GLuint createFragmentShader() {

    // Fragment Shader Object Init/Compile
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* theCode[1];
    theCode[0] = fragmentShaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(fragmentShaderCode);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glShaderSource(fragmentShader, 1, theCode, codeLength);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(fragmentShader, sizeof(eLog), NULL, eLog);
        std::cout << "Error compiling the %d shader: '%s'\n", GL_FRAGMENT_SHADER, eLog;
        exit(1);
    }

    return fragmentShader;
}

unsigned int loadTexture() {
    unsigned int texture1, texture2;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return texture1;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        move_on_y += 1;
        moveCamera(move_on_x, move_on_y, move_on_z);
    }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        move_on_y -= 1;
        moveCamera(move_on_x, move_on_y, move_on_z);
    }
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        move_on_x -= 1;
        moveCamera(move_on_x, move_on_y, move_on_z);
    }
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        move_on_x += 1;
        moveCamera(move_on_x, move_on_y, move_on_z);
    }
    else if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS)
    {
        move_on_z += 1;
        moveCamera(move_on_x, move_on_y, move_on_z);
    }
    else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
    {
        move_on_z -= 1;
        moveCamera(move_on_x, move_on_y, move_on_z);
    }
}

void moveCamera(int move_x, int move_y, int move_z) {
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 Model = glm::mat4(1.0f);
    glm::mat4 View = glm::lookAt(
        glm::vec3(move_x, move_y, move_z), // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(1, 0, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    MVP = Projection * View * Model;
}