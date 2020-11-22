#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtx/transform.hpp>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma warning(disable : 4996)

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 576;
int move_on_x = 0;
int move_on_y = 2;
int move_on_z = 10;

glm::mat4 MVP;

const char* vertexShaderCode = "#version 330 core   \n"
"layout(location = 0) in vec3 aPos;                 \n"
"layout(location = 1) in vec3 aColor;               \n"
"                                                   \n"
"uniform mat4 MVP;                                  \n"
"                                                   \n"
"out vec3 ourColor;                                 \n"
"                                                   \n"
"void main()                                        \n"
"{                                                  \n"
"    gl_Position = MVP * vec4(aPos, 1.0);           \n"
"    ourColor = aColor;          \n"
"}\n\0";

const char* fragmentShaderCode = "#version 330 core     \n"
"out vec4 FragColor;                                    \n"
"                                                       \n"
"in vec3 ourColor;                                      \n"
"                                                       \n"
"                                                       \n"
"void main()                                            \n"
"{                                                      \n"
"    FragColor = vec4(ourColor, 0.15);         \n"
"}\n\0";

float cube[] = {
    -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 
                                      
     1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
                                      
     1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
                                      
     1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
                                      
    -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
                                      
     1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
                                      
    -1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
     1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
                                      
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
     1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
                                      
     1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
     1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
                                      
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
     1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
                                      
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
                                      
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
     1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 1.0f
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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
    Model = glm::scale(Model, glm::vec3(100.0f, 100.0f, 100.0f));
    //int colour = glGetUniformLocation(shaderProgram, "ourColor");
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode
    // Window loop
    while (!glfwWindowShouldClose(window))
    {  
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSetKeyCallback(window, key_callback);
        //Model = glm::rotate(Model, glm::radians(0.05f), glm::vec3(0.0f, 1.0f, 0.0f));
        //MVP = Projection * View * Model;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        // Shaders.use and draw
        glUseProgram(shaderProgram);
        //glUniform4f(colour, 1.0f, 0.0f, 0.0f, 0.0f);
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

bool loadOBJ(
    const char* path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec2>& out_uvs,
    std::vector<glm::vec3>& out_normals
) {
    printf("Loading OBJ file %s...\n", path);

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;


    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
        getchar();
        return false;
    }

    while (1) {

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader

        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
            temp_uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9) {
                printf("File can't be read by our simple parser :-( Try exporting with other options\n");
                fclose(file);
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
        else {
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }

    }

    // For each vertex of each triangle
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {

        // Get the indices of its attributes
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int uvIndex = uvIndices[i];
        unsigned int normalIndex = normalIndices[i];

        // Get the attributes thanks to the index
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        glm::vec2 uv = temp_uvs[uvIndex - 1];
        glm::vec3 normal = temp_normals[normalIndex - 1];

        // Put the attributes in buffers
        out_vertices.push_back(vertex);
        out_uvs.push_back(uv);
        out_normals.push_back(normal);

    }
    fclose(file);
    return true;
}
