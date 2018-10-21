#version 330 core

layout(location = 0) in vec3 xyz;
layout(location = 1) in vec3 mno;
layout(location = 2) in vec2 tex;

out SHADER {
	vec3 fragment_xyz;
	vec3 normal_mno;
	vec2 texture_uv;
} frag;

uniform mat4 vp_matrix;
uniform mat4 model_matrix;
uniform mat4 inver_matrix;

void main() {
	vec4 world_position = model_matrix * vec4(xyz, 1.0);

	frag.fragment_xyz = world_position.xyz;
	frag.texture_uv = tex;
	frag.normal_mno = inver_matrix * mno;

	gl_Position = vp_matrix * world_position;
}