#pragma once

#include "Shape.h"

class Poly : public Shape
{
public:
	Poly();
	~Poly();
	Shape* Clone() override;
	
	const glm::vec2 GetSupport(const glm::vec2 & direction) const;
	const glm::vec2 * GetNormals() const;
	const glm::vec2 GetNormal(const unsigned int index) const;

	const unsigned int GetType() const override;

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
	
	static ShaderBundle s_normal_shader_bundle;
};