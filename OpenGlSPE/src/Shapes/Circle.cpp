#include "Circle.h"

const double PI = acos(-1);

Circle::Circle() :
	Shape(),
	m_radius(1.f)
{
	LOG_INFO("+++   Circle_1.");
}

Circle::Circle(float r, unsigned int verticies_count) :
	Shape(),
	m_radius(r)
{
	m_verticies_count = verticies_count;
	GenVerticiesPositions();
	InitBuffers();
	LOG_INFO("+++   Circle_2.");
}

Circle::~Circle()
{
	LOG_CRITICAL("~~~   Circle.");
}

Shape * Circle::Clone()
{
	Circle *c = new Circle;
	c->SetRadius(m_radius, m_verticies_count);
	c->SetTranslation(m_translation);
	c->SetRotation(m_orientation);
	c->m_material = m_material;
	c->m_shader_bundle = m_shader_bundle;

	return c;
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

const unsigned int Circle::GetType() const
{
	return CircleType;
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