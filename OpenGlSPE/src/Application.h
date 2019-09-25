#pragma once

#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "Graphics\Renderer.h"
#include "Graphics\IndexBuffer.h"
#include "Graphics\VertexBuffer.h"
#include "Graphics\VertexArray.h"
#include "Graphics\VertexBufferLayout.h"
#include "Graphics\Shader.h"
#include "Graphics\Shape.h"
#include "Graphics\View.h"
#include "Graphics\Circle.h"
#include "Graphics\Polygon.h"

#include "FrameTimer.h"

#include "vendor\imgui\imgui.h"
#include "vendor\imgui\imgui_impl_opengl3.h"
#include "vendor\imgui\imgui_impl_glfw.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

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
	Shape* shape;
	Circle* circle;
	Poly* poly;

	GLFWwindow* window;

	FrameTimer ft;
	const double m_update_rate = 25.0;
	const double m_update_time = 1.0 / m_update_rate;
	const unsigned int m_max_frame_skip = 5;

	float t_x, t_y;
	bool b_translate;

	static double cursor_pos_x, cursor_pos_y;
	static double old_cursor_pos_x, old_cursor_pos_y;
	static bool is_RMB_pressed;
};
