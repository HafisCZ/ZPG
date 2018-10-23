#version 330 core

layout(location = 0) in vec3 xyz;

uniform mat4 vp;

uniform mat4 u_model;
uniform vec3 u_color;

out vec3 v_color;
out vec3 v_position;

void main()
{
	gl_Position = vp * u_model * vec4(xyz, 1.0);
	v_position = xyz;
	v_color = u_color;
}