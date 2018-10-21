#version 330 core

layout(location = 0) out vec3 target_position;
layout(location = 1) out vec3 target_normal;
layout(location = 2) out vec4 target_albedo;

in SHADER {
	vec3 fragment_xyz;
	vec3 normal_mno;
	vec2 texture_uv;
} vert;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main() {
	target_position = vert.fragment_xyz;
	target_normal = normalize(vert.normal_mno);
	target_albedo.rgb = texture(texture_diffuse1, vert.texture_uv).rgb;
	target_albedo.a = texture(texture_specular1, vert.texture_uv).r;
}