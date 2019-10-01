#include "Shader.h"
#include "Renderer.h"

Shader::Shader() :
	m_FilePath(""),
	m_RendererID(0)

{
}

Shader::Shader(const std::string & filepath) :
	m_FilePath(filepath),
	m_RendererID(0)
{
	ShaderProgram shaderProgram = ParseShader(filepath);
	m_RendererID = CreateShader(shaderProgram.VertexShader, shaderProgram.FragmentShader);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::SetFilePath(const std::string & filepath)
{
	m_FilePath = filepath;
	ShaderProgram shaderProgram = ParseShader(filepath);
	m_RendererID = CreateShader(shaderProgram.VertexShader, shaderProgram.FragmentShader);
	LOG_INFO("RENDERER ID {1}: {0}", m_RendererID, m_FilePath);
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::setUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const std::string & name, const glm::mat4 & matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

bool Shader::Loaded() const
{
	return m_RendererID;
}

unsigned int Shader::GetUniformLocation(const std::string & name)
{
	if (m_LocationCache.find(name) != m_LocationCache.end()) {
		return m_LocationCache[name];
	}

	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1) {
		LOG_ERROR("Uniform \"{0}\" doesnt exist\n", name);
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
	return{ ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader.\n";
		std::cout << message << "\n";

		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string & vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}