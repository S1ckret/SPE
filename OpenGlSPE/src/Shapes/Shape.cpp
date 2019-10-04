#include "Shape.h"

Shape::Shape() :
	m_translation_mat(glm::ortho(-1.f, 1.f, -1.f, 1.f, -1.0f, 1.0f)),
	m_rotation_mat(glm::ortho(-1.f, 1.f, -1.f, 1.f, -1.0f, 1.0f)),
	m_translation(glm::vec2(0.f, 0.f)),
	m_orientation(0.f),
	m_verticies_count(0.f)
{
	m_shader_bundle.Set();
	LOG_INFO("+++   Shape.");
}

Shape::~Shape()
{
	std::vector<Vertex>().swap(m_verticies);
	LOG_CRITICAL("~~~   Shape.");
}

void Shape::SetMaterial(Material_Table mt, bool connectivity)
{
	m_material.Set(mt);
	m_shader_bundle.Set(mt, connectivity);
}

void Shape::SetVerticies(Vertex * verticies, unsigned int count)
{
	if (m_verticies.size() != count) {
		m_verticies.resize(count);
		m_verticies_count = count;
	}
	for (unsigned int i = 0; i < m_verticies_count; i++) {
		m_verticies[i] = verticies[i];
	}
	InitBuffers();
}

void Shape::SetVerticiesPositions(float * positions, unsigned int count)
{
	if (m_verticies.size() != count) {
		m_verticies.resize(count);
		m_verticies_count = count;
	}
	for (unsigned int i = 0; i < 2 * m_verticies_count; i += 2) {
		m_verticies[i].x = positions[i];
		m_verticies[i].y = positions[i + 1];
	}
	InitBuffers();
}

void Shape::SetVerticiesColors(float * colors, unsigned int count)
{
	if (m_verticies.size() <= count) {
		for (unsigned int i = 0; i < 3 * count; i += 3) {
			m_verticies[i].r = colors[i];
			m_verticies[i].g = colors[i + 1];
			m_verticies[i].b = colors[i + 2];
		}
	}
	InitBuffers();
}

void Shape::Translate(float x, float y)
{
	m_translation += glm::vec2(x, y);
	m_translation_mat = glm::translate(m_translation_mat, glm::vec3(x, y, 0.f));
}

void Shape::Translate(glm::vec2 tr)
{
	m_translation += tr;
	m_translation_mat = glm::translate(m_translation_mat, glm::vec3(tr, 0.f));
}

void Shape::SetTranslation(float x, float y)
{
	m_translation_mat = glm::translate(m_translation_mat, glm::vec3(-m_translation, 0.f));
	m_translation = glm::vec2(x, y);
	m_translation_mat = glm::translate(m_translation_mat, glm::vec3(m_translation, 0.f));
}

void Shape::SetTranslation(glm::vec2 tr)
{
	SetTranslation(tr.x, tr.y);
}

void Shape::Rotate(float d_rad)
{
	m_orientation += d_rad;
	m_rotation_mat = glm::rotate(m_rotation_mat, d_rad, glm::vec3(0.f, 0.f, 1.f));
}

void Shape::SetRotation(float rad)
{
	m_rotation_mat = glm::rotate(m_rotation_mat, -m_orientation, glm::vec3(0.f, 0.f, 1.f));
	m_orientation = rad;
	m_rotation_mat = glm::rotate(m_rotation_mat, m_orientation, glm::vec3(0.f, 0.f, 1.f));
}

const Material & Shape::GetMaterial() const
{
	return m_material;
}

const ShaderBundle & Shape::GetShaderBundle() const
{
	return m_shader_bundle;
}

const glm::vec2 Shape::GetPosition() const
{
	return m_translation;
}

const float Shape::GetOrientation() const
{
	return m_orientation;
}

const unsigned int Shape::GetVerticiesCount() const
{
	return m_verticies_count;
}

const Vertex * Shape::GetVerticies() const
{
	return &m_verticies[0];
}

const glm::mat4 & Shape::GetRotationMat() const
{
	return m_rotation_mat;
}

const glm::mat4 & Shape::GetTranslationMat() const
{
	return m_translation_mat;
}

void Shape::Draw(Renderer & renderer)
{
	m_shader_bundle.shader.Bind();
	m_shader_bundle.shader.setUniformMat4f("model", m_translation_mat * m_rotation_mat);

	renderer.Draw(m_vertex_array, m_shader_bundle, m_verticies.size());
}

void Shape::InitBuffers()
{
	m_vertex_buffer.SetData(&m_verticies[0], m_verticies_count * sizeof(Vertex));

	m_vertex_layout.Clear();
	m_vertex_layout.Push<float>(2);		// positions
	m_vertex_layout.Push<float>(3);		// colors

	m_vertex_array.AddBuffer(m_vertex_buffer, m_vertex_layout);

	m_vertex_buffer.Unbind();
	m_vertex_array.Unbind();
}