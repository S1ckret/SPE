#include "Application.h"



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

//	glfwSwapInterval(1);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using OpenGL %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init("#version 130");
}

Application::~Application()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

void Application::Run()
{
	Init();
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
				std::cout << loops << "\n";
				std::cout << delta_time << "\n";
				std::cout << elapsed_time << "\n";
				std::cout << next_update_time << "\n";
				std::cout << updates_count << "\n";
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
