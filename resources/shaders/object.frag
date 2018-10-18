#version 330 core

layout (location = 0) out vec4 color;

in VS_OUT {
	vec3 position;
	vec3 normal;
	vec2 texture;
} vert;

struct Shadow { samplerCube map; };
struct Light { vec3 pos, amb, dif, spc, clq; };

layout (std140) uniform data_view {
	vec3 u_view;
};

layout (std140) uniform data_light {
	Light u_light;
};

uniform Shadow u_shadow;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

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
	float bias = 0.15;
	
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
	vec3 dtex = vec3(texture(texture_diffuse1, vert.texture));
	vec3 stex = vec3(texture(texture_specular1, vert.texture));

	vec3 lf_dir = normalize(l.pos - fragment);
	float diff_intensity = max(dot(normal, lf_dir), 0.0);

	vec3 halfway = normalize(lf_dir + view);
	float spec_intesity = pow(max(dot(normal, halfway), 0.0), 32.0);

	float dist = length(l.pos - fragment);
	float attenuation = 1.0 / (l.clq.x + l.clq.y * dist + l.clq.z * dist * dist);

	vec3 ambient = attenuation * l.amb * dtex;
	vec3 diffuse = attenuation * l.dif * diff_intensity * dtex;
	vec3 speculr = attenuation * l.spc * spec_intesity * stex;

	return (ambient + (diffuse + speculr));// * (1.0 - processShadow(l.pos)));
}