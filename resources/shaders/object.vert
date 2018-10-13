#version 330 core

layout(location = 0) in vec3 xyz;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 nmo;

layout(std140) uniform data_matrix {
	mat4 vp;
};

uniform mat4 u_model;
uniform mat4 u_inver;

out VS_OUT {
	vec3 position;
	vec3 normal;
	vec2 texture;
} frag;

void main()
{
	gl_Position = vp * u_model * vec4(xyz, 1.0);
	
	frag.position = vec3(u_model * vec4(xyz, 1.0));
	frag.normal = mat3(u_inver) * nmo;
	frag.texture = uv;
}