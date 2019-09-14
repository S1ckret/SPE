#include "Renderer.h"
#include <iostream>

void GLClearErrors() 
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char * function, const char * file, int line) 
{
	while (GLenum error = glGetError()) 
	{
		std::cout << "[OpenGL Error] #" << error << " in " << function << "\nLine: " << line << "\n" << file << "\n";
		return false;
	}
	return true;
}
