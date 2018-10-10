#pragma once

#include "UniformBufferLayout.h"

class UniformBuffer {
	private:
		unsigned int m_handle;
		UniformBufferLayout m_layout;

	public:
		UniformBuffer(const UniformBufferLayout& layout);
		~UniformBuffer();

		void setUniformBlock(unsigned int index, const void* a);

		void bind() const;
		void unbind() const;
};