#shader vertex
#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

out vec4 ourColor;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
	gl_Position = Projection * View * vec4(position, 1.0, 1.0);
	ourColor = vec4(color, 1.0);
};

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;
in vec4 ourColor;

void main()
{
	color = ourColor;
};
