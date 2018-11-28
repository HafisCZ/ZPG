#pragma once

#include "Buffer.h"

class Alignment {
	public:
		static inline const std::size_t alignto(std::size_t size, std::size_t align = 16) {
			return size % align == 0 ? size : size + align - size % align;
		}

};

struct UniformElement {
	std::size_t index;
	std::size_t stride;
	std::size_t size;
};

struct UniformGroup : UniformElement {
	std::vector<UniformElement> elements;

	UniformGroup(std::size_t index, std::size_t stride, std::size_t size) : UniformElement { index, stride, size } { }

	inline const std::size_t getBlockSize() const { return size; }
	inline const std::size_t getItemSize(std::size_t index) const { return elements[index].size; }
	inline const std::size_t getBlockStride(std::size_t offset = 0) const { return stride + elements[offset].stride; }
};

class UniformLayout {
	private:
		std::vector<UniformGroup> _elements;
		std::vector<UniformElement> _items;

		std::size_t _size;
		std::size_t _istr;

		int _step;

	public:
		UniformLayout();

		template <typename T> void push(std::size_t count = 1) {
			std::size_t	bytes = sizeof(T);
			for (std::size_t i = 0; i < count; i++) {
				m_items.push_back({ m_items.size(), m_istr, bytes });
				m_istr += Alignment::alignto(bytes);
			}
		}

		void pack();

		template <typename T> void pack() {
			push<T>();
			pack();
		}

		inline const std::vector<UniformGroup>& getElements() const { return _elements; }
		inline unsigned int getSize() const { return _size; }
};

class UniformBuffer : public Buffer {
	private:
		UniformLayout _layout;

	public:
		UniformBuffer(const UniformLayout& layout);
		~UniformBuffer();

		template <typename T> void setUniformBlock(std::size_t index, const T& a) {
			glBufferSubData(GL_UNIFORM_BUFFER, _layout.getElements()[index].getBlockStride(), _layout.getElements()[index].getBlockSize(), &a);
		}

		template <typename T> void setUniformBlock(std::size_t index, std::size_t position, const T& a) {
			glBufferSubData(GL_UNIFORM_BUFFER, _layout.getElements()[index].getBlockStride(position), _layout.getElements()[index].getItemSize(position), &a);
		}

		void bind();
		void unbind();
};

