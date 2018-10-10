#pragma once

#include <GL/glew.h>

#include <vector>

struct UniformBufferElement {
	unsigned int index, stride, size;
};

class UniformBufferLayout {
	private:
		std::vector<UniformBufferElement> m_elements;

		unsigned int m_size;
		int m_step;

	public:
		UniformBufferLayout() : m_size(0) {
			glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &m_step);
		}

		template <typename T> void push(unsigned int count = 0) {
			unsigned int size = sizeof(T);

			m_elements.push_back({ m_elements.size(), m_size, size });
			m_size += size + (m_step - size % m_step);
		}

		inline const std::vector<UniformBufferElement>& getElements() const { return m_elements; }
		inline unsigned int getSize() const { return m_size; }
};