#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Program.h"

class Renderer {
	private:
	public:
		void clear() const;
		void clear(float r, float g, float b) const;

		void draw(const VertexArray& va, const IndexBuffer& ib, const Program& program) const;
};