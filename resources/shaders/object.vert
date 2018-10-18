#version 330 core

layout(location = 0) in vec3 xyz;
layout(location = 1) in vec3 nmo;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tt;
layout(location = 4) in vec3 bt;

layout(std140) uniform data_matrix { 
	mat4 vp;
};

layout(std140) uniform data_view {
	vec3 view;
};

struct Light { vec3 pos, amb, dif, spc, clq; };
layout(std140) uniform data_light {
	Light light;
};

uniform mat4 u_model;
uniform mat4 u_inver;

out VS_OUT {
	vec3 pos;
	vec2 tex;
	vec3 tlp;
	vec3 tvp;
	vec3 tfp;
	Light light;
} frag;

void main()
{
	gl_Position = vp * u_model * vec4(xyz, 1.0);
	
	frag.pos = vec3(u_model * vec4(xyz, 1.0));
	frag.tex = uv;

	mat3 normal_mat = mat3(u_inver);

	vec3 t = normalize(normal_mat * tt);
	vec3 n = normalize(normal_mat * nmo);
	t = normalize(t - dot(t, n) * n);
	vec3 b = cross(n, t);
	mat3 tbn = transpose(mat3(t, b, n));

	frag.tlp = tbn * light.pos;
	frag.tvp = tbn * view;
	frag.tfp = tbn * frag.pos;

	frag.light = light;
}