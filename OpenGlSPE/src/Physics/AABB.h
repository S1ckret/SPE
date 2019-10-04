#pragma once 
#include "Shapes\Shape.h"

class AABB
{
public:
	AABB();
	const glm::vec2 * GetPositions() const;

	void GenerateVerticies(const Vertex * verticies, const glm::mat4& rotation_mat, int count);
	
	void Draw(Renderer& renderer, const glm::mat4& translation_mat);
private:
	glm::vec2 m_positions[4];

	VertexBuffer m_vertex_buffer;
	VertexArray m_vertex_array;
	VertexBufferLayout m_vertex_layout;
	static ShaderBundle s_shader_bundle;
};