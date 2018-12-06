#version 440 core

layout(location = 0) in vec3 xyz;

uniform mat4 uModel;

void main()
{
	gl_Position = uModel * vec4(xyz, 1.0);
}