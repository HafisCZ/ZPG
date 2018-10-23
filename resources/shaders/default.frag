#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_color;
in vec3 v_position;

void main() 
{
	color = vec4(v_color, 1.0);
}