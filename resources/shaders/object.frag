#version 330 core

layout (location = 0) out vec4 color;

in VS_OUT {
	vec3 position;
	vec3 normal;
	vec2 texture;
} vert;

struct Material { sampler2D diff, spec; };
struct Shadow { samplerCube map; };
struct Light { vec3 pos, amb, dif, spc, clq; };

layout (std140) uniform data_view {
	vec3 u_view;
};

layout (std140) uniform data_light {
	Light u_light;
};

uniform Material u_material;
uniform Shadow u_shadow;

vec3 processLight(Light l, vec3 normal, vec3 view, vec3 fragment);
float processShadow(vec3 position);

vec3 samples[20] = vec3[]
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
	vec3 result = processLight(u_light, normal, view, vert.position);
	
	color = vec4(result, 1.0);
}

float processShadow(vec3 position) 
{
	vec3 len = vert.position - position;
	float current = length(len);

	float shadow = 0.0;
	float bias = 0.05;
	
	float dist = length(u_view - vert.position);
	float disk = (1.0 + (dist / 25.0)) / 25.0;

	for (int i = 0; i < 20; i++) {
		if (current - bias > texture(u_shadow.map, len + samples[i] * disk).r * 25.0) {
			shadow += 1.0;
		}
	}

	return shadow / 20.0;
}

vec3 processLight(Light l, vec3 normal, vec3 view, vec3 fragment) 
{
	vec3 direction = normalize(l.pos - fragment);
	float diff = max(dot(normal, direction), 0.0);

	vec3 halfway = normalize(direction + view);
	float spec = pow(max(dot(normal, halfway), 0.0), 32.0);

	float distance = length(l.pos - fragment);
	float attenuation = 1.0 / (l.clq.x + l.clq.y * distance + l.clq.z * distance * distance);

	vec3 ambient = attenuation * l.amb * vec3(texture(u_material.diff, vert.texture));
	vec3 diffuse = attenuation * l.dif * diff * vec3(texture(u_material.diff, vert.texture));
	vec3 speculr = attenuation * l.spc * spec * vec3(texture(u_material.spec, vert.texture));

	return (ambient + (diffuse + speculr) * (1.0 - processShadow(l.pos)));
}