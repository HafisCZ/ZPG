#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out mat3 TangentSpace;

uniform mat4 uModel;
uniform mat4 uInver;
uniform mat4 uViewProjection;

void main()
{
	vec4 worldPos = uModel * vec4(aPos, 1.0);

	FragPos = worldPos.xyz;
	TexCoords = aTexCoords;
	Normal = mat3(uInver) * aNormal;

	mat3 normalMatrix = mat3(uInver);
	vec3 t = normalize(normalMatrix * aTangent);
	vec3 n = normalize(normalMatrix * aNormal);
	t = normalize(t - dot(t, n) * n);
	vec3 b = cross(t, n);

	TangentSpace = mat3(uModel) * mat3(t, b, n);

    gl_Position = uViewProjection * worldPos;
}

