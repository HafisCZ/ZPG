#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Program.h"

class Renderer {
	public:
		void clear() const;

		void draw(VertexArray& va, IndexBuffer& ib, const Program& program) const;
		void draw(VertexArray& va, std::size_t count, const Program& program) const;
};