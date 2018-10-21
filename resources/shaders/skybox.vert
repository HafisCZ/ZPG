#version 330 core

layout(location = 0) in vec3 xyz;

uniform mat4 proj;
uniform mat4 view;

out vec3 tex;

void main()
{
	tex = xyz;
	vec4 pos = proj * view * vec4(xyz, 1.0);
	gl_Position = pos.xyww;
}