#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_position;

void main() 
{
	color = vec4(1.0, 1.0, 1.0, 1.0);
	color = vec4(vec3(.1 * (v_position.y + 5.0)), 1.0);
}