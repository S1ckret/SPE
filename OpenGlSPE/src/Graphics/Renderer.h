#pragma once

#include "Graphics\VertexArray.h"
#include "Graphics\IndexBuffer.h"
#include "Graphics\Shader.h"
#include "Graphics\View.h"

#include "Physics\Material.h"

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

	void Clear() const;

	void SetView(const View* view);

	void Draw(const VertexArray& va, const IndexBuffer& ib, Shader& shader) const;
	void Draw(const VertexArray& va, Material& material, unsigned int vertex_count) const;
	void Draw(const VertexArray& va, Shader& shader, unsigned int vertex_count) const;
private:
	const View* m_view;
};
