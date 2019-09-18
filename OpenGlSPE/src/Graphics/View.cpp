#include "View.h"

View::View() :
	m_translation({0.f, 0.f, 0.f})
{
	m_view_mat4 = glm::ortho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
}

void View::SetView(const glm::mat4 & view)
{
	m_view_mat4 = view;
}

const glm::mat4 & View::GetView() const
{
	return m_view_mat4;
}

void View::Translate(float x, float y)
{
	m_translation += glm::vec3(x, y, 0.f);
	m_view_mat4 = glm::translate(m_view_mat4, m_translation);
}

void View::SetTranslation(float x, float y)
{
	m_view_mat4 = glm::translate(m_view_mat4, -m_translation);
	m_translation = glm::vec3(x, y, 0.f);
	m_view_mat4 = glm::translate(m_view_mat4, m_translation);
}
