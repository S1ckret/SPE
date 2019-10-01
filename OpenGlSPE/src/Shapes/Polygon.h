#pragma once

#include "Shape.h"

class Poly : public Shape
{
public:
	Poly();
	~Poly();
	Shape* Clone() override;

	void Draw(Renderer& renderer);

private:
	void GenNormals();
	void DrawNormals(Renderer& renderer);
	void InitBuffers() override;
	void InitBuffersNormals();
private:
	std::vector<glm::vec2> m_normals;
	std::vector<glm::vec2> m_normals_draw;
	VertexBuffer m_vertex_buffer_normal;
	VertexArray m_vertex_array_normal;
	VertexBufferLayout m_vertex_layout_normal;
	
	static Shader s_normal_shader;
};