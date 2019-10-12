#pragma once

#include "Graphics\Renderer.h"
#include "Graphics\IndexBuffer.h"
#include "Graphics\VertexBuffer.h"
#include "Graphics\VertexArray.h"
#include "Graphics\VertexBufferLayout.h"
#include "Graphics\Shader.h"
#include "Graphics\View.h"

#include "Shapes\Shape.h"
#include "Shapes\Circle.h"
#include "Shapes\Polygon.h"

#include "Physics\Body.h"
#include "Physics\World.h"

#include "FrameTimer.h"

class Application
{
public:
	Application();
	~Application();

	void Run();
protected:
	void Init();

	void HandleInput();
	void Update(const float dt);

	void HandleEvent();
	void SetEventCallbacks();
	static void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void ButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);

	void Draw();
	void ImGuiDraw();
private:
	void Terminate();

private:
	Renderer* renderer;
	View view;
	Poly* poly;
	World* world;
	Body* body;
	Body* body1;
	Body * circle1, *circle2;
	GLFWwindow* window;

	FrameTimer ft;
	const double m_update_rate = 25.0;
	const double m_update_time = 1.0 / m_update_rate;
	const unsigned int m_max_frame_skip = 5;

	float t_x, t_y, t_rad;
	bool b_translate;
	bool b_rotate;

	static double cursor_pos_x, cursor_pos_y;
	static double old_cursor_pos_x, old_cursor_pos_y;
	static bool is_RMB_pressed;
};
