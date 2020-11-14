// include standard Libraries
#include <stdio.h>
#include <stdlib.h>

// include written classes
#include "Window.h"
// #include "Renderer.h"

int main(void)
{
	try 
	{
		Window window;
		window.draw();
	}
	catch (const char* message) 
	{
		std::cout << message << std::endl;
		return -1;
	}
	return 0;
}