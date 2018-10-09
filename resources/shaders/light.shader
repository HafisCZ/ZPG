#shader vertex 
#version 330 core

layout(std140) uniform shader_matrix{
	mat4 view;
	mat4 proj;
};

uniform mat4 mode;

layout(location = 0) in vec4 position;

void main()
{
	gl_Position = proj * view * mode * position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main() 
{
	color = vec4(1.0, 1.0, 1.0, 1.0);
}