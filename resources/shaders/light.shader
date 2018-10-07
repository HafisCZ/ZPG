#shader vertex 
#version 330 core

struct MVP {
	mat4 mode;
	mat4 view;
	mat4 proj;
};

layout(location = 0) in vec4 position;

uniform MVP u_mvp;

void main()
{
	gl_Position = u_mvp.proj * u_mvp.view * u_mvp.mode * position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main() 
{
	color = vec4(1.0, 1.0, 1.0, 1.0);
}