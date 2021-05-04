#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 u_VP;

void main()
{
	gl_Position = u_VP * vec4(position, 1.0f);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
	color = u_Color;
};