#include "ErrorHandling.h"
void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

/* function to convert decimal error from glGetError to hexadecimal*/
void decToHexa(int n)
{
    // char array to store hexadecimal number 
    char hexaDeciNum[100];

    // counter for hexadecimal number array 
    int i = 0;
    while (n != 0)
    {
        // temporary variable to store remainder 
        int temp = 0;

        // storing remainder in temp variable. 
        temp = n % 16;

        // check if temp < 10 
        if (temp < 10)
        {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
        else
        {
            hexaDeciNum[i] = temp + 55;
            i++;
        }

        n = n / 16;
    }

    // printing hexadecimal number array in reverse order 
    for (int j = i - 1; j >= 0; j--)
        std::cout << hexaDeciNum[j];
}

/* After reading the error that this function returns, go to glew.h
 * and figure out the problem by searching the code
 */
bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Errro] (";
        decToHexa(error);
        std::cout << ")\nFunction:" << function
            << "\nFile:" << file << "\nLine:"
            << line << std::endl;
        return false;
    }
    return true;
}
