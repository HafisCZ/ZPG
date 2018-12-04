#version 330 core

in vec4 FragPos;

uniform vec3 uPosition;
uniform float uFar;

void main() 
{
	gl_FragDepth = length(FragPos.xyz - uPosition) / uFar;
}