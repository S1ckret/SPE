#include "Renderer.h"
#include <iostream>

glm::mat4 PROJ = glm::ortho(-20.f, 20.f, -20.f, 20.f, -1.0f, 1.0f);

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

void Renderer::SetView(const View * view)
{
	m_view = view;
}

void Renderer::Draw(const VertexArray & va, const IndexBuffer & ib, Shader & shader) const
{
	va.Bind();
	ib.Bind();
	shader.setUniformMat4f("view", m_view->GetView());
	shader.setUniformMat4f("projection", PROJ);
	shader.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const VertexArray & va, Material & ma, unsigned int vertex_count)
{
	va.Bind();
	ma.shader.Bind();
	ma.shader.setUniformMat4f("view", m_view->GetView());
	ma.shader.setUniformMat4f("projection", PROJ);
	GLCall(glDrawArrays(ma.draw_type, 0, vertex_count));
}
