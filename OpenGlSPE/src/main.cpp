#include <iostream>

#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

#include "vendor\imgui\imgui.h"
#include"vendor\imgui\imgui_impl_opengl3.h"
#include "vendor\imgui\imgui_impl_glfw.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using OpenGL %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	{
		GLfloat positions[] = {
			-0.5f, -0.5f,
			0.5f, -0.5f,
			0.5f, 0.5f,
			-0.5f, 0.5f
		};

		unsigned int indecies[]{
			0, 1, 2,
			2, 3, 0
		};

		VertexBuffer vb(positions, 8 * sizeof(float));
		VertexArray va;
		VertexBufferLayout vbl;
		vbl.Push<float>(2);
		va.AddBuffer(vb, vbl);
	
		IndexBuffer ib(indecies, 6);
		
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.setUniform4f("u_Color", 0.0, 0.0, 0.5, 1.0);

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui::StyleColorsDark();
		ImGui_ImplOpenGL3_Init("#version 130");

		float r = 0.f;
		float inc = 0.01f;
		// Our state
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();
			shader.setUniform4f("u_Color", r, 0.0, 0.5, 1.0);
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

				ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
				ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
				ImGui::Checkbox("Another Window", &show_another_window);

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

				if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}

			renderer.Draw(va, ib, shader);
			if (r > 1.0) {
				inc = -0.01f;
			}
			else if (r < 0.0) {
				inc = 0.01f;
			}

			r += inc;
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}