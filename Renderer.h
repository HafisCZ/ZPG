#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Program.h"
#include "Texture.h"
#include "gtype.h"

class Renderer {
	public:
		void clear() const;

		void draw(const VertexArray& va, const IndexBuffer& ib, const Program& program) const;
		void draw(const VertexArray& va, std::size_t count, const Program& program) const;
};