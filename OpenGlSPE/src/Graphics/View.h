#pragma once 

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class View
{
public:
	View();
	
	void SetView(const glm::mat4& view);
	const glm::mat4& GetView() const;

	void Translate(float x, float y);
	void SetTranslation(float x, float y);

	/*	TODO:
	void Zoom();
	void SetZoom();

	void Rotate();
	void SetRotation();
	*/
private:
	
	glm::mat4 m_view_mat4;
	glm::vec2 m_translation;
};