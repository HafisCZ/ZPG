#version 440 core

layout(location = 0) in vec3 aPos;

uniform mat4 uTranslatedView;
uniform mat4 uProjection;

out vec3 TexCoords;

void main()
{
	TexCoords = aPos;
	gl_Position = vec4(uProjection * uTranslatedView * vec4(aPos, 1.0)).xyww;
}