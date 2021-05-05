#version 400

layout (location = 0) in vec3 vPos;

uniform mat4 u_Transform;
uniform mat4 u_ViewProjection;

out vec4 color;
out vec2 uv;

void main()
{
   color = uColor;
   uv = vPos.xy;
   gl_Position = uVP * uTransform * vec4(vPos, 1.0); 
}
