#version 440 core

layout (triangles) in;
layout (triangle_strip, max_vertices=90) out;

uniform mat4 uFaces[30];
uniform int uFaceCount;

out vec4 FragPos;

void main()
{
	for (int face = 0; face < min(uFaceCount, 30); face++) {
		gl_Layer = face;

		for (int i = 0; i < 3; i++) {
			FragPos = gl_in[i].gl_Position;

			gl_Position = uFaces[face] * FragPos;
		
			EmitVertex();
		}

		EndPrimitive();
	}
}