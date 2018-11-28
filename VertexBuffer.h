#pragma once

#include "Buffer.h"

class VertexBuffer : public Buffer {
	public:
		VertexBuffer(void_cptr ptr, std::size_t size);
		~VertexBuffer();

		void bind();
		void unbind();
};