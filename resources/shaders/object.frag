#version 330 core

layout (location = 0) out vec4 color;

in VS_OUT {
	vec3 position;
	vec3 normal;
	vec2 texture;
} vert;

struct Material {
	float shine, far;
	sampler2D diff, spec;
	samplerCube cube;
};

struct LightPoint {
	vec3 pos, amb, dif, spc, clq;
};

layout (std140) uniform lights_point {
	int u_light_point_cnt;
	LightPoint u_light_point[10];
};

uniform vec3 u_view;
uniform Material u_material;

vec3 calculateLightPoint(LightPoint l, vec3 normal, vec3 view, vec3 fragment);
float calculateShadow(LightPoint l);

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

void main() 
{
	vec3 normal = normalize(vert.normal);
	vec3 view = normalize(u_view - vert.position);

	vec3 result = vec3(0.0);

	for (int i = 0; i < u_light_point_cnt; i++) {
		result += calculateLightPoint(u_light_point[i], normal, view, vert.position);
	}

	color = vec4(result, 1.0);
}

float calculateShadow(LightPoint l) {
	vec3 ftol = vert.position - l.pos;
	float current = length(ftol);

	float shadow = 0.0;
	float bias = 0.05;
	int samples = 20;
	float viewdist = length(u_view - vert.position);
	float diskrad = (1.0 + (viewdist / u_material.far)) / 25.0;
	for (int i = 0; i < samples; ++i) {
		float closest = texture(u_material.cube, ftol + gridSamplingDisk[i] * diskrad).r;
		closest *= u_material.far;
		if (current - bias > closest) shadow += 1.0;
	}
	shadow /= float(samples);
	return shadow;
}

vec3 calculateLightPoint(LightPoint l, vec3 normal, vec3 view, vec3 fragment) {
	vec3 direction = normalize(l.pos - fragment);
	float diff = max(dot(normal, direction), 0.0);

	vec3 reflection = reflect(-direction, normal);
	//float spec = pow(max(dot(view, reflection), 0.0), u_material.shine);

	vec3 halfway = normalize(direction + view);
	float spec = pow(max(dot(normal, halfway), 0.0), u_material.shine);

	float dist = length(l.pos - fragment);
	float attn = 1.0 / (l.clq.x + l.clq.y * dist + l.clq.z * dist * dist);

	vec3 ambient = attn * l.amb * vec3(texture(u_material.diff, vert.texture));
	vec3 diffuse = attn * l.dif * diff * vec3(texture(u_material.diff, vert.texture));
	vec3 specular = attn * l.spc * spec * vec3(texture(u_material.spec, vert.texture));

	float shadow = calculateShadow(l);

	return (ambient + (diffuse + specular) * (1.0 - shadow));
}