#version 330 core

layout(location = 0) in vec3 xyz;

uniform mat4 model;

void main()
{
	gl_Position = model * vec4(xyz, 1.0);
}