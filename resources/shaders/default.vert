#version 330 core

layout(location = 0) in vec3 xyz;

layout(std140) uniform data_matrix {
	mat4 vp;
};

uniform mat4 u_model;

out vec3 v_position;

void main()
{
	gl_Position = vp * u_model * vec4(xyz, 1.0);
	v_position = xyz;
}