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

	void Draw();
	void ImGuiDraw();
private:
	void Terminate();

private:
	Renderer renderer;
	View view;
	Shape* shape;
	GLFWwindow* window;

	FrameTimer ft;
	const double m_update_rate = 25.0;
	const double m_update_time = 1.0 / m_update_rate;
	const unsigned int m_max_frame_skip = 5;

	float zoom = 1.f;
	float left, right, bottom, top;

	float t_x, t_y;
	bool b_translate;
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
