#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light {
    vec3 Position;
    vec3 Color;

	float Diffuse;
	float Specular;

    float Linear;
    float Quadratic;
    float Radius;
};

const int MAX_LIGHTS = 64;

uniform Light lights[MAX_LIGHTS];
uniform int lightCount;

uniform vec3 viewPos;

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

			vec3 diffuseColor = (lights[i].Color * lights[i].Diffuse * diffuse) * attenuation;
			vec3 specularColor = (lights[i].Color * lights[i].Specular * specular) * attenuation;

			color += (Diffuse * diffuseColor + Specular * specularColor);
        }
    }    

    FragColor = vec4(color, 1.0);
}

