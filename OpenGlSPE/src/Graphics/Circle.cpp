#include "Circle.h"

const double PI = acos(-1);

Circle::Circle() :
	Shape(),
	m_radius(1.f)
{
	m_material.draw_type = GL_LINE_LOOP;
	LOG_INFO("+++   Circle_1.");
}

Circle::Circle(float r, unsigned int verticies_count) :
	Shape(),
	m_radius(r)
{
	m_verticies_count = verticies_count;
	m_material.draw_type = GL_LINE_LOOP;
	GenVerticiesPositions();
	InitBuffers();
	LOG_INFO("+++   Circle_2.");
}

Circle::~Circle()
{
	LOG_CRITICAL("~~~   Circle.");
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
	if (m_verticies.size() != m_verticies_count)
	{
		m_verticies.resize(m_verticies_count);
	}
	double step = 2.f * PI / static_cast<double>(m_verticies_count);
	for (unsigned int i = 0; i < m_verticies_count; i++)
	{
		m_verticies[i].x = m_radius * cos(step * i);
		m_verticies[i].y = m_radius * sin(step * i);
		m_verticies[i].color = { 1.f, 1.f, 1.f };
	}
}