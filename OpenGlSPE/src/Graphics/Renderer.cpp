#include "Renderer.h"

glm::mat4 PROJ = glm::ortho(-20.f, 20.f, -20.f, 20.f, -1.0f, 1.0f);

void GLClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char * function, const char * file, int line)
{
	while (GLenum error = glGetError())
	{
		LOG_ERROR("[OpenGL Error] # {0:d} in {1} Line: {2}\n", error, function, line);
		return false;
	}
	return true;
}

Renderer::Renderer()
{
	m_normal_shader.SetFilePath("res/shaders/Normal.shader");
}

void Renderer::Clear() const
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
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
	ib.Unbind();
}

void Renderer::Draw(const VertexArray & va, Material & ma, unsigned int vertex_count) const
{
	va.Bind();
	ma.shader.Bind();
	ma.shader.setUniformMat4f("view", m_view->GetView());
	ma.shader.setUniformMat4f("projection", PROJ);
	GLCall(glDrawArrays(GL_LINE_LOOP, 0, vertex_count));
}

void Renderer::DrawNormal(const VertexArray & va, unsigned int vertex_count, const glm::mat4 & model_mat)
{
	va.Bind();
	m_normal_shader.Bind();
	m_normal_shader.setUniformMat4f("model", model_mat);
	m_normal_shader.setUniformMat4f("view", m_view->GetView());
	m_normal_shader.setUniformMat4f("projection", PROJ);
	GLCall(glDrawArrays(GL_LINES, 0, vertex_count));
}