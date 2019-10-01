#pragma once
#include "Physics\Material.h"

struct ShaderProgram {
	std::string VertexShader;
	std::string FragmentShader;
};

class Shader
{
public:
	Shader();
	Shader(const std::string& filepath);
	~Shader();

	void SetFilePath(const std::string& filepath);
	void Bind() const;
	void Unbind() const;

	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

	bool Loaded() const;
private:
	unsigned int GetUniformLocation(const std::string& name);

	ShaderProgram ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string & vertexShader, const std::string& fragmentShader);

private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_LocationCache;
};

struct ShaderBundle
{
	unsigned int draw_type;
	Shader shader;

	ShaderBundle() : draw_type(GL_LINE_LOOP)
	{
	//	shader.SetFilePath("res/shaders/BasicColor.shader");
	}
	void Set(Material_Table Type = Material_Table::Static, bool conectivity = 1)
	{
		if (conectivity) {
			switch (Type) 
			{
			default: shader.SetFilePath("res/shaders/BasicColor.shader");
				break;
			}
		}
		else
		{
			shader.SetFilePath("res/shaders/BasicColor.shader");
		}
	}
};
