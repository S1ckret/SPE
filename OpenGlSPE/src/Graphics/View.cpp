#include "View.h"
#include "Log.h"
View::View() :
	m_translation(glm::vec2(0.f, 0.f)),
	m_zoom(1.f),
	m_zoom_sensitivity(0.1f),
	m_translate_sensitivity(0.1f),
	m_left(-m_zoom),
	m_right(m_zoom),
	m_top(-m_zoom),
	m_bot(m_zoom),
	m_view_mat4(glm::ortho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f))
{
	
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
	glm::vec2 transtation = (m_view_mat4 / glm::vec4(x, y, 0.f, 0.f)) * m_translate_sensitivity;
	m_translation += transtation;
	m_view_mat4 = glm::translate(m_view_mat4, glm::vec3(transtation, 0.f));
}

void View::SetTranslation(float x, float y)
{
	m_view_mat4 = glm::translate(m_view_mat4, glm::vec3(-m_translation, 0.f));
	m_translation = glm::vec2(x, y);
	m_view_mat4 = glm::translate(m_view_mat4, glm::vec3(m_translation, 0.f));
}

void View::Zoom(float zoom)
{
	SetZoom(m_zoom + zoom * m_zoom_sensitivity);
}

void View::SetZoom(float zoom)
{
	if(zoom > 0.f) {
		m_zoom = zoom;
		LOG_WARN("m_zoom {0}", m_zoom);
		m_view_mat4 = glm::ortho(-m_zoom, m_zoom, -m_zoom, m_zoom, -1.f, 1.f);
		m_view_mat4 = glm::translate(m_view_mat4, glm::vec3(m_translation, 0.f));
	}
}
