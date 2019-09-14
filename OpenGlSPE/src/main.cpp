#include <iostream>

#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

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

		float r = 0.f;
		float inc = 0.01f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			shader.Bind();
			shader.setUniform4f("u_Color", r, 0.0, 0.5, 1.0);

			va.Bind();
			ib.Bind();
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
			if (r > 1.0) {
				inc = -0.01f;
			}
			else if (r < 0.0) {
				inc = 0.01f;
			}

			r += inc;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

	}
	glfwTerminate();
	return 0;
}