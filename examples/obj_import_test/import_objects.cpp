﻿#include <vector>
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
int move_on_y = 0;
int move_on_z = 3;
bool texture = true;

glm::mat4 MVP;

const char* vertexShaderCode_old = "#version 330 core   \n"
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

const char* fragmentShaderCode_old = "#version 330 core     \n"
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

const char* fragmentShaderCode = "#version 330 core"
"                                                                        \n"
"// Interpolated values from the vertex shaders                          \n"
"in vec2 UV;                                                             \n"
"                                                                        \n"
"// Ouput data                                                           \n"
"out vec3 color;                                                         \n"
"                                                                        \n"
"// Values that stay constant for the whole mesh.                        \n"
"uniform sampler2D myTextureSampler;                                     \n"
"                                                                        \n"
"void main() {                                                           \n"
"                                                                        \n"
"    // Output color = color of the texture at the specified UV          \n"
"    color = texture(myTextureSampler, UV).rgb;                          \n"
"}\n\0";



const char* vertexShaderCode = "#version 330 core \n"
"// Input vertex data, different for all executions of this shader.     \n"
"layout(location = 0) in vec3 vertexPosition_modelspace;                \n"
"layout(location = 1) in vec2 vertexUV;                                 \n"
"                                                                       \n"
"// Output data ; will be interpolated for each fragment.               \n"
"out vec2 UV;                                                           \n"
"                                                                       \n"
"// Values that stay constant for the whole mesh.                       \n"
"uniform mat4 MVP;                                                      \n"
"                                                                       \n"
"void main() {                                                          \n"
"                                                                       \n"
"    // Output position of the vertex, in clip space : MVP * position   \n"
"    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);            \n"
"                                                                       \n"
"    // UV of the vertex. No special space for this one.                \n"
"    UV = vertexUV;\n"
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
bool loadOBJ(
    const char* path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec2>& out_uvs,
    std::vector<glm::vec3>& out_normals
);

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

    // SUZZANA
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals; // Won't be used at the moment.
    bool res = loadOBJ("suzi.obj", vertices, uvs, normals);
    if (res == true) {
        std::cout << "Loaded .obj \n";
    }
    
    glEnable(GL_DEPTH_TEST);
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    
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

    //unsigned int texture1 = loadTexture();
    //glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    unsigned int texture1 = loadTexture();
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (texture)
        {   
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        glfwSetKeyCallback(window, key_callback);
        Model = glm::rotate(Model, glm::radians(0.05f), glm::vec3(0.0f, 1.0f, 0.0f));
        MVP = Projection * View * Model;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        // Shaders.use and draw
        glUseProgram(shaderProgram);
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,                  // attribute
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
            1,                                // attribute
            2,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Cleanup
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
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
    unsigned char* data = stbi_load("texture.jpg", &width, &height, &nrChannels, 0);
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
    else if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        texture = !texture;
        std::cout << "texture " << texture << "\n";
        if (!texture) {
            std::cout << "binded to default texture \n";
        }
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
    std::cout << "recorded " << move_x << ":" << move_y << ":" << move_z << "\n";
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