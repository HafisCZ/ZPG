#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Program.h"
#include "Texture.h"

class Renderer {
	public:
		void clear() const;

		void draw(VertexArray& va, IndexBuffer& ib, const Program& program) const;
		void draw(VertexArray& va, std::size_t count, const Program& program) const;

		void bindTexture(int id, unsigned int slot) const { Texture::repo(id).bind(slot); }
};