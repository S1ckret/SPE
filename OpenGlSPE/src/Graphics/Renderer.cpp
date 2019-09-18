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

Renderer::Renderer()
{
}

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray & va, const IndexBuffer & ib, const Shader & shader) const
{
	va.Bind();
	ib.Bind();
	shader.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const VertexArray & va, const Material & ma, unsigned int vertex_count)
{
	va.Bind();
	ma.shader.Bind();
	GLCall(glDrawArrays(ma.draw_type, 0, vertex_count));
}
