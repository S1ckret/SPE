#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string & filepath) :
	m_FilePath(filepath),
	m_RendererID(0)
{
	ShaderProgram shaderProgram = ParseShader(filepath);
	m_RendererID = CreateShader(shaderProgram.VertexShader, shaderProgram.FragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::setUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
{

	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniformMat4f(const std::string & name, const glm::mat4 & matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

unsigned int Shader::GetUniformLocation(const std::string & name)
{
	if (m_LocationCache.find(name) != m_LocationCache.end()) {
		return m_LocationCache[name];
	}

	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1) {
		std::cout << "Uniform [" << name << "] doesnt exist\n";
	}
	m_LocationCache[name] = location;
	return location;
}


ShaderProgram Shader::ParseShader(const std::string& filepath) 
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

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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

unsigned int Shader::CreateShader(const std::string & vertexShader, const std::string& fragmentShader)
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
