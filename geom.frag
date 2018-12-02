#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;

in vec3 FragPos;
in vec3 Normal;
in vec3 TangentNormal;

uniform bool uDiffuseEnabled;
uniform bool uSpecularEnabled;
uniform bool uNormalEnabled;

uniform sampler2D uDiffuse;
uniform sampler2D uSpecular;
uniform sampler2D uNormal;

void main()
{    
    gPosition = FragPos;

	if (uNormalEnabled) {
		gNormal = texture(uNormal, TexCoords).rgb * 2.0 - 1.0;
	} else {
		if (uDiffuseEnabled) {
			gNormal = Normal;
		} else {
			gNormal = vec3(0.0, 1.0, 0.0);
		}
	}
	
    gAlbedoSpec.rgb = uDiffuseEnabled ? texture(uDiffuse, TexCoords).rgb : vec3(1.0);
    gAlbedoSpec.a = uSpecularEnabled ? texture(uSpecular, TexCoords).r : 0.5;
}

