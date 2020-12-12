#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
out vec4 v_Color;

uniform mat4 u_ModelView;
uniform mat4 u_Projection;
uniform vec4 u_Color;

vec4 ComputeLight(vec3 light_dir, vec4 light_col, vec3 normal, vec3 half_angle,
	vec4 diffuse, vec4 spec, float spec_size)
{
	float light_int = max(dot(normal, -light_dir), 0.0f);
	vec4 lambert = diffuse * light_col * light_int;

	float spec_int = pow(max(dot(normal, half_angle), 0.0f), spec_size);
	vec4 phong = spec * light_col * spec_int;

	return lambert + phong;
};

void main()
{
	gl_Position = u_Projection * u_ModelView * vec4(position, 1.0f);
	v_Color = 1 / u_Color;
};

#shader fragment
#version 330 core

in vec4 v_Color;
layout(location = 0) out vec4 fragcolor;

void main()
{
	fragcolor = v_Color;
};