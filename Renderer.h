#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define glCall(x) glClearError();\
	x;\
	ASSERT(glLogCall(#x, __FILE__, __LINE__))

void glClearError();
bool glLogCall(const char* function, const char* file, int line);

class Renderer {
	private:
		int m_fcnt = 0;
		int m_ffps = 0;
		double m_flst = 0;
	public:
		void clear() const;
		void clear(float r, float g, float b) const;

		void sample();
		int getFps() const;

		void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};