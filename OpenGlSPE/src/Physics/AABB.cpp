#include "AABB.h"

ShaderBundle AABB::s_shader_bundle;

AABB::AABB()
{
	if (!s_shader_bundle.shader.Loaded()) 
	{
		s_shader_bundle.shader.SetFilePath("res/shaders/AABB.shader");
		s_shader_bundle.draw_type = GL_LINE_LOOP;
	}
	LOG_INFO("+++ AABB.");

	m_vertex_buffer.SetData(&m_positions[0], 4 * sizeof(glm::vec2));

	m_vertex_layout.Clear();
	m_vertex_layout.Push<float>(2);		// positions

	m_vertex_array.AddBuffer(m_vertex_buffer, m_vertex_layout);

	m_vertex_buffer.Unbind();
	m_vertex_array.Unbind();

}

const glm::vec2 * AABB::GetPositions() const
{
	return &m_positions[0];
}

void AABB::GenerateVerticies(const Vertex * verticies, const glm::mat4& rotation_mat, int count)
{
	glm::vec2 min(1000000.f, 1000000.f);
	glm::vec2 max(-1000000.f, -1000000.f);
	
	for (unsigned int i = 0; i < count; i++)
	{
		glm::vec4 vertex = rotation_mat * glm::vec4(verticies[i].position, 0.f, 0.f);
		if (vertex.x > max.x) max.x = vertex.x;
		if (vertex.y > max.y) max.y = vertex.y;
		if (vertex.x < min.x) min.x = vertex.x;
		if (vertex.y < min.y) min.y = vertex.y;
	}
	m_positions[0] = min;
	m_positions[1] = glm::vec2(max.x, min.y);
	m_positions[2] = max;
	m_positions[3] = glm::vec2(min.x, max.y);
	m_vertex_buffer.SetNewData(&m_positions[0], 4 * sizeof(glm::vec2));
}

void AABB::Draw(Renderer & renderer, const glm::mat4 & translation_mat)
{
	s_shader_bundle.shader.Bind();
	s_shader_bundle.shader.setUniformMat4f("model", translation_mat);

	renderer.Draw(m_vertex_array, s_shader_bundle, 4);
}
