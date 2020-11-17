#pragma once
#include <GL/glew.h>
#include <iostream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

/* After reading the error that this function returns, go to glew.h
 * and figure out the problem by searching the code
 */
bool GLLogCall(const char* function, const char* file, int line);

/* function to convert decimal error
 * from glGetError to hexadecimal
 */
void decToHexa(int n);

