#pragma once 

#include "Shapes\Shape.h"
#include "Shapes\Circle.h"
#include "Shapes\Polygon.h"

class AABB
{
public:
	AABB();
	~AABB();
	const glm::vec2 * GetPositions() const;

	void SetShape(const Shape* shape);

	void GenerateVerticies(Circle * circle);
	void GenerateVerticies(Poly * poly);
	void GenerateVerticies(const Vertex * verticies, const glm::mat4& rotation_mat, int count);
	
//	void ApplyMat4(glm::mat4 matrix);
	bool isOverlaping(const AABB* rhs) const;

	void Draw(Renderer& renderer, const glm::mat4& translation_mat);
private:
	glm::vec2 m_positions[4];
	const Shape* m_shape_ptr;

	VertexBuffer m_vertex_buffer;
	VertexArray m_vertex_array;
	VertexBufferLayout m_vertex_layout;
	static ShaderBundle s_shader_bundle;

	friend bool isAabbOverlaping(AABB * a, AABB * b);

};

void ApplyMat4(glm::vec2 * positions, const glm::vec2& translation_vector, unsigned int count);
bool isAabbOverlaping(AABB * a, AABB * b);
void CopyPositions(glm::vec2 * lhs, const glm::vec2 * rhs, unsigned int count);