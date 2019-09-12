#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "GL\glew.h"
#include "GLFW\glfw3.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearErrors();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearErrors() 
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char * function, const char * file, int line) 
{
	while (GLenum error = glGetError()) 
	{
		std::cout << "[OpenGL Error] #" << error << " in " << function << "\nLine: " << line << "\n" << file << "\n";
		return false;
	}
	return true;
}

struct ShaderProgram {
	std::string VertexShader;
	std::string FragmentShader;
};

static ShaderProgram ParseShader(const std::string& filepath) 
{
	enum class ShaderType 
	{
		None = -1,
		Vertex = 0,
		Fragment = 1
	} type;
	std::stringstream ss[2];
	std::ifstream file(filepath);
	std::string line;
	while (std::getline(file, line)) {
		if (line.find("#shader") != std::string::npos) 
		{
			if (line.find("vertex") != std::string::npos) 
			{
				type = ShaderType::Vertex;
			}
			else if (line.find("fragment") != std::string::npos) 
			{
				type = ShaderType::Fragment;
			}
		}
		else 
		{
			ss[(int)type] << line << '\n';
		}
	}
	file.close();
	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader.\n";
		std::cout << message << "\n";

		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string & vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using OpenGL %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

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

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indecies, GL_STATIC_DRAW);

	ShaderProgram shaderProgram = ParseShader("res/shaders/Basic.shader");
	std::cout << "Vertex" << "\n";
	std::cout << shaderProgram.VertexShader << "\n";
	std::cout << "Fragment" << "\n";
	std::cout << shaderProgram.FragmentShader << "\n";

	unsigned int shader = CreateShader(shaderProgram.VertexShader, shaderProgram.FragmentShader);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		
		
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}