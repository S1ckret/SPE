#include "Shape.h"
#include <iostream>

Shape::Shape()
{
}

Shape::~Shape()
{
	std::vector<Vertex>().swap(m_verticies);
}

void Shape::SetVerticies(Vertex * verticies, unsigned int count)
{
	if (m_verticies.size() != count) {
		m_verticies.resize(count);
	}
	for (unsigned int i = 0; i < count; i++) {
		m_verticies[i] = verticies[i];
	}
	InitBuffers();
}

void Shape::SetVerticiesPositions(float * positions, unsigned int count)
{
	if (m_verticies.size() != count) {
		m_verticies.resize(count);
	}
	for (unsigned int i = 0; i < 2 * count; i += 2) {
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

void Shape::Draw(Renderer & renderer)
{
//	m_material.shader.setUniformMat4f(m_model_mat4);
	renderer.Draw(m_vertex_array, m_material, m_verticies.size());
}

void Shape::InitBuffers()
{
	m_vertex_buffer.SetData(&m_verticies[0], m_verticies.size() * sizeof(Vertex));

	m_vertex_layout.Clear();
	m_vertex_layout.Push<float>(2);		// positions
	m_vertex_layout.Push<float>(3);		// colors

	m_vertex_array.AddBuffer(m_vertex_buffer, m_vertex_layout);

	m_vertex_buffer.Unbind();
	m_vertex_array.Unbind();
}
