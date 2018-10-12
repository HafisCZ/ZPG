#version 330 core

layout(location = 0) in vec3 xyz;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 nmo;

layout(std140) uniform shader_matrix {
	mat4 vp;
	mat4 light;
};

uniform mat4 model;
uniform mat4 model_tinv;

out VS_OUT {
	vec3 position;
	vec3 normal;
	vec2 texture;
} frag;

void main()
{
	gl_Position = vp * model * vec4(xyz, 1.0);
	
	frag.position = vec3(model * vec4(xyz, 1.0));
	frag.normal = mat3(model_tinv) * nmo;
	frag.texture = uv;
}