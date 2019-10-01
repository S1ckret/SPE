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

void Renderer::Draw(const VertexArray & va, const IndexBuffer & ib, ShaderBundle & shader_bundle) const
{
	va.Bind();
	ib.Bind();
	BindShader(shader_bundle.shader);
	GLCall(glDrawElements(shader_bundle.draw_type, ib.getCount(), GL_UNSIGNED_INT, nullptr));
	ib.Unbind();
}

void Renderer::Draw(const VertexArray & va, ShaderBundle & shader_bundle, unsigned int vertex_count) const
{
	va.Bind();
	BindShader(shader_bundle.shader);
	GLCall(glDrawArrays(shader_bundle.draw_type, 0, vertex_count));
}

void Renderer::BindShader(Shader & shader) const
{
	shader.Bind();
	shader.setUniformMat4f("view", m_view->GetView());
	shader.setUniformMat4f("projection", PROJ);
}
