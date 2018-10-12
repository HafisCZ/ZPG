#pragma once

#include <GL/glew.h>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray : Buffer {
	private:
		unsigned int m_handle;
	public:
		VertexArray();
		~VertexArray();

		void addBuffer(VertexBuffer& vb, const VertexBufferLayout& layout);

		void bind();
		void unbind();
};