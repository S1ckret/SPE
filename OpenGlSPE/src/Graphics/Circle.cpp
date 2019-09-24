#include "Circle.h"

const double PI = acos(-1);

Circle::Circle() : 
	Shape(),
	m_radius(1.f),
	m_verticies_count(12.f)
{
	m_material.draw_type = GL_LINE_LOOP;
}

Circle::Circle(float r, unsigned int verticies_count) : 
	Shape(),
	m_radius(r),
	m_verticies_count(verticies_count)
{
	m_material.draw_type = GL_LINE_LOOP;
	GenVerticiesPositions();
	InitBuffers();
}

Circle::~Circle()
{
}

void Circle::SetRadius(float r, unsigned int verticies_count)
{
	m_radius = r;
	m_verticies_count = verticies_count;
	GenVerticiesPositions();
	InitBuffers();
}

float Circle::GetRadius() const
{
	return m_radius;
}

void Circle::GenVerticiesPositions()
{
	unsigned int size = m_verticies.size();
	if (size != m_verticies_count)
	{
		m_verticies.resize(m_verticies_count);
		size = m_verticies_count;
	}
	double step = 2.f * PI / static_cast<double>(size);
	for (unsigned int i = 0; i < size; i++)
	{
		m_verticies[i].x = m_radius * cos(step * i);
		m_verticies[i].y = m_radius * sin(step * i);
		m_verticies[i].color = {1.f, 1.f, 1.f};
	}

}
