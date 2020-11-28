#include "Object.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma warning(disable : 4996)


Object::Object(){}

Object::Object(int objectId)
{
    if (objectId == TYPE_SUZI)
    {
        if (!loadOBJ("./res/models/Extras/suzi.obj"))
        {
            throw("Loading SUZI failed");
        }
    }
    else if (objectId == TYPE_SPHERE)
    {
        if (!loadOBJ("./res/models/sphere.obj"))
        {
            throw("Loading SPHERE failed");
        }
    }
    else if (objectId == TYPE_CILINDER)
    {
        if (!loadOBJ("./res/models/cilinder.obj"))
        {
            throw("Loading ICOS failed");
        }
    }
    else if (objectId == TYPE_CUBE)
    {
        if (!loadOBJ("./res/models/cube.obj"))
        {
            throw("Loading CUBE failed");
        }
    }
    bindVBO();
}

/*
Object::~Object() {
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
}
*/

void Object::bindVAO()
{
    glBindVertexArray(vertexArrayID);
}

void Object::unbindVAO()
{
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Object::bindVBO()
{
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, m_uvs.size() * sizeof(glm::vec2), &m_uvs[0], GL_STATIC_DRAW);

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
}

bool Object::loadOBJ(const char* path)
{
    printf("$Obj :: Loading OBJ file %s...\n", path);

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
        
        m_vertices.push_back(vertex);
        m_uvs.push_back(uv);
        m_normals.push_back(normal);

    }
    fclose(file);
    return true;
}

void Object::loadTexture(const char* path) {

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
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
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
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
}

// Used on keycallback to enable/disable texture
void Object::switchTexture()
{
    enableTexture = !enableTexture;
    std::cout << "$Obj :: EnableTexture set to: " << enableTexture << "\n";
}

void Object::randomRGB()
{
    srand(time(NULL));
    float r = ((double)rand() / (RAND_MAX));
    float g = ((double)rand() / (RAND_MAX));
    float b = ((double)rand() / (RAND_MAX));
    std::cout << "$Obj :: RGB value set to: " << r << " " << g << " " << b << "\n";
    color = glm::vec3(r, g, b);
}

void Object::moveFrom(char* input)
{   
    // Returns first token  
    char* token = strtok(input, ",");

    // Keep printing tokens while one of the 
    // delimiters present in str[]. 
    while (token != NULL)
    {
        printf("%s\n", token);
        token = strtok(NULL, ",");
    }
    std::cout << token[1];
    //x = 0, y = 0;
    //if (z > 10) {
    //    z -= 0.1;
    //}else if (z < -10){
    //    z += 0.1;
    //}
    //modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
}

void Object::moveObject(glm::vec3& translation) {
    modelMatrix = glm::translate(modelMatrix, translation);
    translation = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Object::bounceObject(glm::vec3 direction) {
    modelMatrix = glm::translate(modelMatrix, direction);
}

void Object::detectCollision(glm::mat4 shpereMatrix)
{
    glm::vec4 landingPosition = modelMatrix * glm::vec4(initialDirection, 1.0f);
    //glm::vec4 shperePosition = shpereMatrix;

    if (landingPosition.y >= 100) {
        initialDirection = glm::vec3(initialDirection.x, -initialDirection.y, initialDirection.z);
    }
    else if (landingPosition.y <= 0) {
        initialDirection = glm::vec3(initialDirection.x, -initialDirection.y, initialDirection.z);
    }
    else if (landingPosition.x >= 100) {
        initialDirection = glm::vec3(-initialDirection.x, initialDirection.y, initialDirection.z);
    }
    else if (landingPosition.x <= 0) {
        initialDirection = glm::vec3(-initialDirection.x, initialDirection.y, initialDirection.z);
    }
    else if (landingPosition.z >= 100) {
        initialDirection = glm::vec3(initialDirection.x, initialDirection.y, -initialDirection.z);
    }
    else if (landingPosition.z <= 0) {
        initialDirection = glm::vec3(initialDirection.x, initialDirection.y, -initialDirection.z);
    }
}

GLuint Object::getVertexArrayID() {
    return vertexArrayID;
}