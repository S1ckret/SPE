#shader vertex
#version 330 core

layout(location = 0) in vec2 position;

uniform mat4 u_MVP;

out vec3 our_Color;

void main()
{
	gl_Position = u_MVP * position;
};

#shader fragment
#version 330 core
layout(location = 0) out vec4 color;

uniform vec4 u_Color;
void main()
{
	color = u_Color;
};
