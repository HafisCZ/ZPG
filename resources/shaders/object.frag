#version 330 core

layout (location = 0) out vec4 color;

struct Light { vec3 pos, amb, dif, spc, clq; };
in VS_OUT {
	vec3 nmo;
	vec3 pos;
	vec2 tex;
	vec3 tlp;
	vec3 tvp;
	vec3 tfp;
	Light light;
} vert;

uniform bool texture_diffuse_enable;
uniform sampler2D texture_diffuse;

uniform bool texture_specular_enable;
uniform sampler2D texture_specular;

uniform bool texture_normal_enable;
uniform sampler2D texture_normal;

uniform bool texture_height_enable;
uniform sampler2D texture_height;

uniform samplerCube texture_shadow;

vec3 samples[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float process_shading() 
{
	vec3 len = vert.pos - vert.light.pos;
	float current = length(len);

	float shadow = 0.0;
	float bias = 0.15;
	
	float dist = length(vert.tvp - vert.tfp);
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

	vec3 diffuse_color = texture_diffuse_enable ? texture(texture_diffuse, uv).rgb : vec3(1.0);
	vec3 specular_color = texture_specular_enable ? texture(texture_specular, uv).rgb : vec3(0.0);
	vec3 normal = texture_normal_enable ? normalize(texture(texture_normal, uv).rgb * 2.0 - 1.0) : vert.nmo;

	vec3 light_direction = normalize(vert.tlp - vert.tfp);
	float diffuse_intensity = max(dot(light_direction, normal), 0.0);

	vec3 view_direction = normalize(vert.tvp - vert.tfp);
	vec3 halfway_direction = normalize(light_direction + view_direction);
	float specular_intensity = pow(max(dot(normal, halfway_direction), 0.0), 32.0);

	float distance_from_light = length(vert.tlp - vert.tfp);
	float attenuation = 1.0 / (vert.light.clq.x + vert.light.clq.y * distance_from_light + vert.light.clq.z * distance_from_light * distance_from_light);

	vec3 ambient = attenuation * vert.light.amb * diffuse_color;
	vec3 diffuse = attenuation * vert.light.dif * diffuse_color * diffuse_intensity;
	vec3 specular = attenuation * vert.light.spc * specular_color * specular_intensity;

	return (ambient + (diffuse + specular) * (1.0 - process_shading()));
}

void main() 
{
	color = vec4(process_lightning(), 1.0);
}

