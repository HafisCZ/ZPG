#pragma once

#include <GL/glew.h>

class IndexBuffer {
	private:
		unsigned int m_handle;
		unsigned int m_count;

	public:
		IndexBuffer(const void* data, unsigned int count);
		~IndexBuffer();

		void bind();
		void unbind();

		inline unsigned int getCount() const { return m_count; }
};