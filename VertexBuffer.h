#pragma once

#include <GL/glew.h>

#include "Buffer.h"

class VertexBuffer : Buffer {
	private:
		unsigned int m_handle;

	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void bind();
		void unbind();
};