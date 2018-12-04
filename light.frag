#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform samplerCube gShadowPoint;
uniform sampler2D gShadowVector;

struct Light {
    vec3 Position;
    vec3 Color;

	float Diffuse;
	float Specular;

    float Linear;
    float Quadratic;
    float Radius;

	bool Shadow;
};

struct LightVector {
	vec3 Direction;
	vec3 Color;
	float Diffuse;
	float Specular;

	bool Enabled;
};

struct LightAmbient {
	vec3 Color;
	float Ambient;

	bool Enabled;
};


const int MAX_LIGHTS = 64;

uniform Light lights[MAX_LIGHTS];
uniform int lightCount;

uniform LightVector vector;
uniform LightAmbient ambient;

uniform vec3 viewPos;

// shadow samples
vec3 samples[20] = vec3[]
(
	vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
	vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
	vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
	vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
	vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
);

float getFragmentShadow(vec3 fragPos, vec3 lightPos) {
	vec3 distance = fragPos - lightPos;
	float current = length(distance);

	float shadow = 0.0;
	float bias = 0.5;

	float dist = length(viewPos - lightPos);
	float disk = (1.0 + (dist / 100.0)) / 25.0;

	for (int i = 0; i < 20; i++) {
		if (current - bias > texture(gShadowPoint, distance + samples[i] * disk).r * 100.0) {
			shadow += 1.0;
		}
	}

	return shadow / 20.0;
} 

vec3 getVectorLight(vec3 Diffuse, float Specular, vec3 Normal, vec3 ViewDir, LightVector Light) {
	vec3 lightDir = normalize(-Light.Direction);
	float diffuse = max(dot(Normal, lightDir), 0.0);

	vec3 halfDir = normalize(lightDir + ViewDir);
	float specular = pow(max(dot(Normal, halfDir), 0.0), 32.0);

	return (Diffuse * diffuse * Light.Diffuse * Light.Color + Specular * specular * Light.Specular * Light.Color);
}

void main()
{
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;

    vec3 color = vec3(0.0);
    vec3 viewDir = normalize(viewPos - FragPos);

    for(int i = 0; i < min(lightCount, MAX_LIGHTS); ++i)
    {
        float distance = length(lights[i].Position - FragPos);

        if (distance < lights[i].Radius)
        {
			vec3 lightDir = normalize(lights[i].Position - FragPos);
			float diffuse = max(dot(lightDir, Normal), 0.0);

			vec3 halfDir = normalize(lightDir + viewDir);
			float specular = pow(max(dot(Normal, halfDir), 0.0), 32.0);

			float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
			
			float intensity = lights[i].Shadow ? 1.0 - getFragmentShadow(FragPos, lights[i].Position) : 1.0;

			vec3 diffuseColor = (lights[i].Color * lights[i].Diffuse * diffuse) * attenuation;
			vec3 specularColor = (lights[i].Color * lights[i].Specular * specular) * attenuation;

			color += (Diffuse * diffuseColor + Specular * specularColor) * intensity;
        }
    }

	if (vector.Enabled) {
		color += getVectorLight(Diffuse, Specular, Normal, viewDir, vector);
	}

	if (ambient.Enabled) {
		color += Diffuse * ambient.Color * ambient.Ambient;
	}

    FragColor = vec4(color, 1.0);
}

