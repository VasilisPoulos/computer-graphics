#include <stdio.h>
#include <stdlib.h>

#include "MainWindow.h"

int main(void)
{
	try 
	{
		MainWindow window;
		window.display();
	}
	catch (const char* message) 
	{
		std::cout << message << std::endl;
		return -1;
	}
	return 0;
}