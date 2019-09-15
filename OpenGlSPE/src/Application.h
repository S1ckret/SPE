#pragma once

#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "Graphics\Renderer.h"
#include "Graphics\IndexBuffer.h"
#include "Graphics\VertexBuffer.h"
#include "Graphics\VertexArray.h"
#include "Graphics\VertexBufferLayout.h"
#include "Graphics\Shader.h"

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
	void Draw();
	void ImGuiDraw();
private:
	GLFWwindow* window;
};