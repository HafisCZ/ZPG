#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

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