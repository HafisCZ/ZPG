#version 330 core

layout(location = 0) in vec3 xyz;

layout(std140) uniform shader_matrix {
	mat4 vp;
};

uniform mat4 model;

void main()
{
	gl_Position = vp * model * vec4(xyz, 1.0);
}