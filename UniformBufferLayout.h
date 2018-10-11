#pragma once

#include <GL/glew.h>

#include <vector>

struct UniformBufferItem {
	unsigned int index, stride, size;
};

struct UniformBufferElement {
	unsigned int index, stride, size;
	std::vector<UniformBufferItem> items;

	inline const std::size_t blockSize() const { return size; }
	inline const std::size_t itemSize(unsigned int index) const { return items[index].size; }

	inline const std::size_t blockStride(unsigned int offset = 0) const { return stride + items[offset].stride; }
};

class UniformBufferLayout {
	private:
		std::vector<UniformBufferElement> m_elements;
		std::vector<UniformBufferItem> m_items;

		std::size_t m_size;
		std::size_t m_istr;

		int m_step;

	public:
		UniformBufferLayout() : m_size(0), m_istr(0) {
			glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &m_step);
		}

		template <typename T> void push(unsigned int count = 1) {
			std::size_t	bytes = sizeof(T);

			for (unsigned int i = 0; i < count; i++) {
				m_items.push_back({ m_items.size(), m_istr, bytes });
				m_istr += alignto(bytes);
			}
		}

		void pack() {
			UniformBufferElement ube = { m_elements.size(), m_size, m_istr };
			ube.items.insert(ube.items.end(), m_items.begin(), m_items.end());

			m_elements.push_back(ube);
			m_items.clear();

			m_size += alignto(m_istr, m_step);
			m_istr = 0;
		}

		template <typename T> void pack() {
			push<T>();
			pack();
		}

		inline const std::vector<UniformBufferElement>& getElements() const { return m_elements; }
		inline unsigned int getSize() const { return m_size; }

	private:
		static inline const std::size_t alignto(std::size_t size, std::size_t align = 16) {
			return size % align == 0 ? size : size + align - size % align;
		}
};