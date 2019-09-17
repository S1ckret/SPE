#pragma once 
#include "GL\glew.h"

#include <vector>

#include "glm\glm.hpp"

struct Vertex
{
	union {
		struct{
			float x, y;
			float r, g ,b;
		};
		struct{
			glm::fvec2 position;
			glm::fvec3 color;
		};
	};
};

class Shape
{
public:
	Shape();
	~Shape();

	void SetVerticies(Vertex* verticies, unsigned int count);
	void SetVerticiesPositions(float* positions, unsigned int count);
	void SetVerticiesColors(float* colors, unsigned int count);
private:
	std::vector<Vertex> m_verticies;
	glm::fvec2 m_centeroid;
	VertexBuffer m_vertex_buffer;
	VertexArray m_vertex_array;
	VertexBufferLayout m_vertex_layout;

	Material m_material;
};