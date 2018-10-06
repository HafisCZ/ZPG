#shader vertex 
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_texCoord;
out vec3 v_normal;
out vec3 v_fragCoord;

uniform mat4 u_m;
uniform mat4 u_v;
uniform mat4 u_p;

void main()
{
	gl_Position = u_p * u_v * u_m * position;

	v_texCoord = texCoord;
	v_normal = mat3(transpose(inverse(u_m))) * normal;
	v_fragCoord = vec3(u_m * position);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_fragCoord;

uniform vec3 u_lightColor;
uniform vec3 u_lightCoord;
uniform vec3 u_viewCoord;

uniform sampler2D u_texture;

void main() 
{
	float ambientIntensity = 0.1;
	vec3 cambient = ambientIntensity * u_lightColor;

	vec4 ctexture = texture(u_texture, v_texCoord);
	
	vec3 norm = normalize(v_normal);
	vec3 light = normalize(u_lightCoord - v_fragCoord);
	float diff = max(dot(norm, light), 0.0);
	vec3 cdiffuse = diff * u_lightColor;

	float specIntensity = 0.5;
	vec3 view = normalize(u_viewCoord - v_fragCoord);
	vec3 viewReflect = reflect(-light, norm);
	float spec = pow(max(dot(view, viewReflect), 0.0), 64);
	vec3 cspecular = specIntensity * spec * u_lightColor;

	color = vec4(cdiffuse + cambient + cspecular, 1.0) * ctexture;
}