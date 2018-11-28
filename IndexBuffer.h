#pragma once

#include "Buffer.h"

class IndexBuffer : public Buffer {
	private:
		std::size_t _indices;

	public:
		IndexBuffer(void_cptr ptr, std::size_t size);
		~IndexBuffer();

		void bind();
		void unbind();

		inline std::size_t getCount() const { return _indices; }
};