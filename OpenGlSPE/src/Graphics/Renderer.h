#pragma once

#include "GL\glew.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

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
	// To Do: Material instead of shader
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void Draw(const VertexArray& va, const Material& ma, unsigned int vertex_count);
private:

};