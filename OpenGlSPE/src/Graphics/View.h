#pragma once

class View
{
public:
	View();

	void SetView(const glm::mat4& view);
	const glm::mat4& GetView() const;

	void Translate(float x, float y);
	void SetTranslation(float x, float y);

	void Zoom(float zoom);
	void SetZoom(float zoom);

	/*	TODO:
	void Rotate();
	void SetRotation();
	*/
private:

	glm::vec2 m_translation;

	float m_zoom;
	float m_zoom_sensitivity;
	float m_translate_sensitivity;
	float m_left, m_right;
	float m_bot, m_top;

	glm::mat4 m_view_mat4;
};