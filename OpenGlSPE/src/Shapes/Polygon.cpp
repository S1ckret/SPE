#include "Polygon.h"

Shader Poly::s_normal_shader;

Poly::Poly()
{
	if (!s_normal_shader.Loaded()) {
		s_normal_shader.SetFilePath("res/shaders/Normal.shader");
		
	}
	LOG_INFO("+++   Poly.");
}

Poly::~Poly()
{
	LOG_CRITICAL("~~~   Poly.");
}

Shape * Poly::Clone()
{
	return new Poly(*this);
}

void Poly::Draw(Renderer & renderer)
{
	Shape::Draw(renderer);
	DrawNormals(renderer);
}

void Poly::GenNormals()
{
	if (m_normals.size() != m_verticies_count)
	{
		m_normals.resize(m_verticies_count);
		m_normals_draw.resize(m_verticies_count * 2);
	}
	unsigned int i = 0;
	do
	{
		unsigned int next = (i + 1) % m_verticies_count;

		glm::vec2 side = m_verticies[next].position - m_verticies[i].position;
		m_normals[i] = glm::normalize(glm::vec2(side.y, -side.x));

		m_normals_draw[i * 2] = (m_verticies[next].position + m_verticies[i].position) * 0.5f;
		m_normals_draw[i * 2 + 1] = m_normals[i] + m_normals_draw[i * 2];

		i = next;
	} while (i);
}

void Poly::DrawNormals(Renderer& renderer)
{
	s_normal_shader.Bind();
	s_normal_shader.setUniformMat4f("model", this->m_translation_mat * this->m_rotation_mat);
	renderer.Draw(m_vertex_array_normal, s_normal_shader, m_verticies_count * 2);
}

void Poly::InitBuffers()
{
	Shape::InitBuffers();
	InitBuffersNormals();
}

void Poly::InitBuffersNormals()
{
	GenNormals();
	m_vertex_buffer_normal.SetData(&m_normals_draw[0], 2 * m_verticies_count * sizeof(glm::vec2));

	m_vertex_layout_normal.Clear();
	m_vertex_layout_normal.Push<float>(2);		// positions

	m_vertex_array_normal.AddBuffer(m_vertex_buffer_normal, m_vertex_layout_normal);

	m_vertex_buffer_normal.Unbind();
	m_vertex_array_normal.Unbind();
}