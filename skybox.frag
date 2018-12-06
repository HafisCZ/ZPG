#version 440 core

layout(location = 0) out vec4 gColor;

in vec3 TexCoords;

uniform samplerCube uDiffuse;

void main() 
{
	gColor = texture(uDiffuse, TexCoords);
}