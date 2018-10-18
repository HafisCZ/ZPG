#pragma once

#include <GL/glew.h>

class VertexBuffer {
	private:
		unsigned int m_handle;

	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void bind();
		void unbind();
};