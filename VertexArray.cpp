#include "VertexArray.h"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &m_handle);
	glBindVertexArray(m_handle);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &m_handle);
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	bind();
	vb.bind();

	const auto& elements = layout.getElements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::bind() const {
	glBindVertexArray(m_handle);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}