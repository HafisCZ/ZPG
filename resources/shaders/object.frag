#version 330 core

#include library.glsl

layout (location = 0) out vec4 color;

struct Light { vec3 pos, amb, dif, spc; vec2 clq; };
in VS_OUT {
	vec3 nmo;
	vec3 pos;
	vec3 view;
	vec3 nor;
	vec2 tex;
	vec3 tlp;
	vec3 tvp;
	vec3 tfp;
	Light light;
} vert;

uniform vec3 u_color;

uniform bool texture_diffuse_enable;
uniform sampler2D texture_diffuse;

uniform bool texture_specular_enable;
uniform sampler2D texture_specular;

uniform bool texture_normal_enable;
uniform sampler2D texture_normal;

uniform bool texture_height_enable;
uniform sampler2D texture_height;

uniform samplerCube texture_shadow;

float process_shading() 
{
	vec3 len = vert.pos - vert.light.pos;
	float current = length(len);

	float shadow = 0.0;
	float bias = 0.15;
	
	float dist = texture_diffuse_enable ? length(vert.tvp - vert.tfp) : length(vert.view - vert.pos);
	float disk = (1.0 + (dist / 25.0)) / 25.0;

	for (int i = 0; i < 20; i++) {
		if (current - bias > texture(texture_shadow, len + samples[i] * disk).r * 25.0) {
			shadow += 1.0;
		}
	}

	return shadow / 20.0;
}

vec3 process_lightning() {
	vec2 uv = vert.tex;

	vec3 diffuse_color = texture_diffuse_enable ? texture(texture_diffuse, uv).rgb : u_color;
	vec3 specular_color = texture_specular_enable ? texture(texture_specular, uv).rgb : (texture_diffuse_enable ? vec3(0.0) : vec3(1.0));
	vec3 normal = texture_normal_enable ? normalize(texture(texture_normal, uv).rgb * 2.0 - 1.0) : (texture_diffuse_enable ? vert.nmo : vert.nor);

	vec3 light_direction = texture_diffuse_enable ? normalize(vert.tlp - vert.tfp) : normalize(vert.light.pos - vert.pos);
	float diffuse_intensity = max(dot(light_direction, normal), 0.0);

	vec3 view_direction = texture_diffuse_enable ? normalize(vert.tvp - vert.tfp) : normalize(vert.view - vert.pos);
	vec3 halfway_direction = normalize(light_direction + view_direction);
	float specular_intensity = pow(max(dot(normal, halfway_direction), 0.0), 32.0);

	float distance_from_light = texture_diffuse_enable ? length(vert.tlp - vert.tfp) : length(vert.light.pos - vert.pos);
	float attenuation = 1.0 / (1.0 + vert.light.clq.x * distance_from_light + vert.light.clq.y * distance_from_light * distance_from_light);

	vec3 ambient = attenuation * vert.light.amb * diffuse_color;
	vec3 diffuse = attenuation * vert.light.dif * diffuse_color * diffuse_intensity;
	vec3 specular = attenuation * vert.light.spc * specular_color * specular_intensity;

	return (ambient + (diffuse + specular) * (1.0 - process_shading()));
}

void main() 
{
	color = vec4(process_lightning(), 1.0);
}

