#version 330 core

in vec4 FragPos;

uniform vec3 u_smap;
uniform float u_smaf;

void main() 
{
	gl_FragDepth = length(FragPos.xyz - u_smap) / u_smaf;
}