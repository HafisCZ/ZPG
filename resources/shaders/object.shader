#shader vertex 
#version 330 core

struct MVP {
	mat4 mode, view, proj;
};

layout(location = 0) in vec4 xyzw;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 nmo;

out vec2 v_texture;
out vec3 v_normal;
out vec3 v_fragment;

uniform MVP u_mvp;

void main()
{
	gl_Position = u_mvp.proj * u_mvp.view * u_mvp.mode * xyzw;

	v_texture = uv;
	v_normal = mat3(transpose(inverse(u_mvp.mode))) * nmo;
	v_fragment = vec3(u_mvp.mode * xyzw);
}

#shader fragment
#version 330 core

struct Material {
	float shine;
	sampler2D smp2, spc2;
};

struct DLight {
	vec3 dir, amb, dif, spc;
};

struct PLight {
	float kc, kl, kq;
	vec3 pos, amb, dif, spc;
};

struct SLight {
	float kc, kl, kq, cutoff1, cutoff2;
	vec3 pos, dir, amb, dif, spc;
};

layout(location = 0) out vec4 color;

in vec3 v_normal;
in vec3 v_fragment;
in vec2 v_texture;

uniform vec3 u_view;
uniform Material u_material;

uniform DLight u_dlight;
uniform PLight u_plight[1];
uniform SLight u_slight;

vec3 getDLight(DLight l, vec3 normal, vec3 view);
vec3 getPLight(PLight l, vec3 normal, vec3 view, vec3 fragment);
vec3 getSLight(SLight l, vec3 normal, vec3 view, vec3 fragment);

void main() 
{
	vec3 normal = normalize(v_normal);
	vec3 view = normalize(u_view - v_fragment);

	vec3 result = getDLight(u_dlight, normal, view);
	for (int i = 0; i < 1; i++) {
		result += getPLight(u_plight[i], normal, view, v_fragment);
	}

	result += getSLight(u_slight, normal, view, v_fragment);

	color = vec4(result, 1.0);
}

vec3 getDLight(DLight l, vec3 normal, vec3 view) {
	vec3 direction = normalize(-l.dir);
	float diff = max(dot(normal, direction), 0.0);

	vec3 reflection = reflect(-direction, normal);
	float spec = pow(max(dot(view, reflection), 0.0), u_material.shine);

	vec3 ambient = l.amb * vec3(texture(u_material.smp2, v_texture));
	vec3 diffuse = l.dif * diff * vec3(texture(u_material.smp2, v_texture));
	vec3 specular = l.spc * spec * vec3(texture(u_material.spc2, v_texture));

	return (ambient + diffuse + specular);
}

vec3 getPLight(PLight l, vec3 normal, vec3 view, vec3 fragment) {
	vec3 direction = normalize(l.pos - fragment);
	float diff = max(dot(normal, direction), 0.0);

	vec3 reflection = reflect(-direction, normal);
	float spec = pow(max(dot(view, reflection), 0.0), u_material.shine);

	float dist = length(l.pos - fragment);
	float attn = 1.0 / (l.kc + l.kl * dist + l.kq * dist * dist);

	vec3 ambient = attn * l.amb * vec3(texture(u_material.smp2, v_texture));
	vec3 diffuse = attn * l.dif * diff * vec3(texture(u_material.smp2, v_texture));
	vec3 specular = attn * l.spc * spec * vec3(texture(u_material.spc2, v_texture));

	return (ambient + diffuse + specular);
}

vec3 getSLight(SLight l, vec3 normal, vec3 view, vec3 fragment) {
	vec3 direction = normalize(l.pos - fragment);
	float diff = max(dot(normal, direction), 0.0);

	vec3 reflection = reflect(-direction, normal);
	float spec = pow(max(dot(view, reflection), 0.0), u_material.shine);

	float dist = length(l.pos - fragment);
	float attn = 1.0 / (l.kc + l.kl * dist + l.kq * dist * dist);

	float theta = dot(direction, normalize(-l.dir));
	float epsilon = l.cutoff1 - l.cutoff2;
	float ints = clamp((theta - l.cutoff2) / epsilon, 0.0, 1.0);

	vec3 ambient = ints * attn * l.amb * vec3(texture(u_material.smp2, v_texture));
	vec3 diffuse = ints * attn * l.dif * diff * vec3(texture(u_material.smp2, v_texture));
	vec3 specular = ints * attn * l.spc * spec * vec3(texture(u_material.spc2, v_texture));

	return (ambient + diffuse + specular);
}