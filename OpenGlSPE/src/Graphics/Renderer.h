#pragma once

#include "GL\glew.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "View.h"
#include "Physics\Material.h"
#include "Log.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearErrors();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearErrors();

bool GLLogCall(const char * function, const char * file, int line);

class Renderer
{
public:
	Renderer();
	// To Do: Material instead of shader
	void Clear() const;

	void SetView(const View* view);

	void Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader) const;
	void Draw(const VertexArray& va, Material& ma, unsigned int vertex_count);
	void DrawNormal(const VertexArray & va, unsigned int vertex_count, const glm::mat4& model_mat);
private:
	const View* m_view;
	Shader m_normal_shader;
};
