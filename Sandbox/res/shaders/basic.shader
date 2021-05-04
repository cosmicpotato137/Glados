#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;
out vec2 v_TexCoord;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(position, 1.0f);
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;

void main()
{
	color = u_Color;
};