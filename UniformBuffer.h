#pragma once

#include "UniformBufferLayout.h"

#include "Buffer.h"

class UniformBuffer : Buffer {
	private:
		unsigned int m_handle;
		UniformBufferLayout m_layout;

	public:
		UniformBuffer(const UniformBufferLayout& layout);
		~UniformBuffer();

		template <typename T> void setUniformBlock(unsigned int index, const T& a) {
			glBufferSubData(GL_UNIFORM_BUFFER, m_layout.getElements()[index].blockStride(), m_layout.getElements()[index].blockSize(), &a);
		}

		template <typename T> void setUniformBlock(unsigned int index, unsigned int position, const T& a) {
			glBufferSubData(GL_UNIFORM_BUFFER, m_layout.getElements()[index].blockStride(position), m_layout.getElements()[index].itemSize(position), &a);
		}

		void bind();
		void unbind();
};