#pragma once

#include <GL/glew.h>

#include <vector>

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type) {
		switch (type) {
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}

		return 0;
	}
};

class VertexBufferLayout {
	private:
		std::vector<VertexBufferElement> m_elements;
		unsigned int m_stride;
	public:
		VertexBufferLayout() : m_stride(0) {
	
		}

		template<typename T> void push(unsigned int count) {
			static_assert(false);
		}

		template<> void push<float>(unsigned int count) {
			m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
			m_stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
		}

		template<> void push<unsigned int>(unsigned int count) {
			m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
			m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
		}

		template<> void push<unsigned char>(unsigned int count) {
			m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
			m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
		}

		inline const std::vector<VertexBufferElement>& getElements() const { return m_elements; }
		inline unsigned int getStride() const { return m_stride; }

		static VertexBufferLayout DEFAULT_PNTTB() {
			VertexBufferLayout vbl;

			vbl.push<float>(3);
			vbl.push<float>(3);
			vbl.push<float>(2);
			vbl.push<float>(3);
			vbl.push<float>(3);

			return vbl;
		}

		static VertexBufferLayout DEFAULT_PNT() {
			VertexBufferLayout vbl;

			vbl.push<float>(3);
			vbl.push<float>(3);
			vbl.push<float>(2);

			return vbl;
		}

		static VertexBufferLayout DEFAULT_PN() {
			VertexBufferLayout vbl;

			vbl.push<float>(3);
			vbl.push<float>(3);

			return vbl;
		}
};