#include "Application.h"

double Application::cursor_pos_x = 0.0;
double Application::cursor_pos_y = 0.0;

Application::Application() :
	left(-zoom),
	right(zoom),
	bottom(-zoom),
	top(zoom),
	t_x(0.f),
	t_y(0.f),
	b_translate(0)
{
	if (!glfwInit())
		terminate();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		terminate();
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
//	glfwSwapInterval(1);
	Log::Init();

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		LOG_ERROR(glewGetErrorString(err));
	}
	LOG_INFO("Using OpenGL {0}", glGetString(GL_VERSION));
	LOG_INFO("Using GLEW {0}", glewGetString(GLEW_VERSION));

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init("#version 130");
}

Application::~Application()
{
	Terminate();
}

void Application::Run()
{
	Init();
	SetEventCallbacks();
	double elapsed_time = ft.GetElapsedTime();
	double next_update_time = 0.0;
	ft.Clear();
	FrameTimer second_timer;
	second_timer.Clear();
	unsigned int updates_count = 0;
	while (!glfwWindowShouldClose(window))
	{

		HandleInput();
		const double delta_time = ft.Mark();
		elapsed_time = ft.GetElapsedTime();
		double second = second_timer.GetElapsedTime();
		unsigned int loops = 0;
		while (elapsed_time >= next_update_time && loops < m_max_frame_skip) 
		{
			next_update_time += m_update_time;
			loops++;
			updates_count++;
			if (second > 1.0) 
			{
				second_timer.Clear();
				second = 0.0;
				LOG_TRACE("Loop_N = {0}, D_T = {1}, Elapsed_T = {2}, Next_Up_T = {3}, Up_N = {4}", loops, delta_time, elapsed_time, next_update_time, updates_count);
				updates_count = 0;
			}


			Update(delta_time);
		}



		view.SetView(glm::ortho(left, right, bottom, top, -1.f, 1.f));
		renderer.Clear();
		Draw();
		ImGuiDraw();
		glfwSwapBuffers(window);
		HandleEvent();
	}

}

void Application::Init()
{
	shape = new Shape;
	Vertex verticies[] = {
		{-5.f, -5.f, 1.f, 0.f, 0.f},
		{5.f, -5.f, 0.f, 1.f, 0.f},
		{0.f, 5.f, 0.f, 0.f, 1.f},
	};
	shape->SetVerticies(verticies, 3);
	renderer.SetView(&view);
}

void Application::HandleInput()
{
}

void Application::Update(const float dt)
{
}

void Application::HandleEvent()
{
	glfwPollEvents();
}

void Application::SetEventCallbacks()
{
	glfwSetKeyCallback(window, &Application::KeyCallback);
	glfwSetScrollCallback(window, &Application::ScrollCallback);
	glfwSetMouseButtonCallback(window, &Application::ButtonCallback);
	glfwSetCursorPosCallback(window, &Application::CursorPosCallback);
}

void Application::KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	Application* app = static_cast<Application *>(glfwGetWindowUserPointer(window));
	switch (action) 
	{
	case GLFW_PRESS:
	{
		if (key == GLFW_KEY_ESCAPE) {
			LOG_WARN("Terminating...");
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
		break;
	default:
		break;
	}
}

void Application::ScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
	Application* app = static_cast<Application *>(glfwGetWindowUserPointer(window));

}

void Application::ButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	Application* app = static_cast<Application *>(glfwGetWindowUserPointer(window));
	switch (action) 
	{
	case GLFW_PRESS:
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT) 
		{
			LOG_WARN("Mouse_Pos:   x:{0}    y{1}", cursor_pos_x, cursor_pos_y);
		}
	}
		break;

	case GLFW_RELEASE:
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT) 
		{
			LOG_WARN("Mouse_Pos:   x:{0}    y{1}", cursor_pos_x, cursor_pos_y);
		}
	}
	
	break;

	}
}

void Application::CursorPosCallback(GLFWwindow * window, double xpos, double ypos)
{
	Application* app = static_cast<Application *>(glfwGetWindowUserPointer(window));
	glfwGetCursorPos(window, &cursor_pos_x, &cursor_pos_y);
}

void Application::Draw()
{
	shape->Draw(renderer);
}

void Application::ImGuiDraw()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	{
		ImGui::Begin("Hello, world!");   
		ImGui::SliderFloat("zoom", &zoom, -10.f, 10.f);
		left = -zoom;
		right = zoom;
		bottom = -zoom;
		top = zoom;
		//ImGui::SliderFloat("left", &left, -10.f, 10.f);
		//ImGui::SliderFloat("right", &right, -10.f, 10.f);
		//ImGui::SliderFloat("bottom", &bottom, -10.f, 10.f);
		//ImGui::SliderFloat("top", &top, -10.f, 10.f);
		if (ImGui::Button("Translate")) 
		{
			shape->Translate(1.f, 1.f);
		}
		ImGui::Checkbox("SetTranslation", &b_translate);
		if (b_translate) 
		{
			shape->SetTranslation(t_x, t_y);
			ImGui::SliderFloat("translate X: ", &t_x, -10.f, 10.f);
			ImGui::SliderFloat("translate Y: ", &t_y, -10.f, 10.f);
		}

		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::Terminate()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}
