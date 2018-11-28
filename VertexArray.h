#pragma once

#include "VertexBuffer.h"

class VertexLayout {
	private:
		struct VertexElement {
			std::size_t type;
			std::size_t count;
			unsigned char normal;
		};

		std::vector<VertexElement> _elements;
		std::size_t _stride;

	public:
		VertexLayout() : _stride(0) {}

		static std::size_t getSize(std::size_t type);

		template <typename T> void push(std::size_t count) { static_assert(false); }

		template <> void push<float>(std::size_t count) {
			_elements.push_back({ GL_FLOAT, count, GL_FALSE });
			_stride += count * getSize(GL_FLOAT);
		}

		template <> void push<unsigned int>(std::size_t count) {
			_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
			_stride += count * getSize(GL_UNSIGNED_INT);
		}

		template <> void push<unsigned char>(std::size_t count) {
			_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
			_stride += count * getSize(GL_UNSIGNED_BYTE);
		}

		inline const std::vector<VertexElement>& getElements() const { return _elements; }
		inline std::size_t getStride() { return _stride; }

		static VertexLayout getPNTTB();
		static VertexLayout getPNT();
		static VertexLayout getPN();
		static VertexLayout getPT();
		static VertexLayout getP();
};


class VertexArray : public Buffer {
	public:
		VertexArray();
		~VertexArray();

		void addBuffer(VertexBuffer& vbo, VertexLayout vbl);

		void bind();
		void unbind();
};
