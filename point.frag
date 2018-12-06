#version 440 core

in vec4 FragPos;

uniform vec3 uPosition[5];

uniform float uFar;

void main() 
{
	gl_FragDepth = length(FragPos.xyz - uPosition[gl_Layer / 6]) / uFar;
}