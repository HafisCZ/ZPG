#version 330 core

layout(location = 0) in vec3 xyz;

uniform mat4 vp;

uniform mat4 u_model;

void main()
{
	gl_Position = vp * u_model * vec4(xyz, 1.0);
}