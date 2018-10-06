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

struct Material {
	vec3 ambt;
	vec3 diff;
	vec3 spec;
	
	float shine;
};

struct Light {
	vec3 post;
	vec3 ambt;
	vec3 diff;
	vec3 spec;
};

layout(location = 0) out vec4 color;

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_fragCoord;

uniform vec3 u_view;
uniform Material u_material;
uniform Light u_light;

uniform sampler2D u_texture;

void main() 
{
	vec3 ambt = u_light.ambt * u_material.ambt;

	vec3 norm = normalize(v_normal);
	vec3 lightVec = normalize(u_light.post - v_fragCoord);

	float diff = max(dot(norm, lightVec), 0.0);
	vec3 dffu = u_light.diff * (diff * u_material.diff);

	vec3 viewVec = normalize(u_view - v_fragCoord);
	vec3 reflVec = reflect(-lightVec, norm);

	float spec = pow(max(dot(viewVec, reflVec), 0.0), u_material.shine);
	vec3 spcr = u_light.spec * (spec * u_material.spec);

	color = vec4(ambt + dffu + spcr, 1.0);
}