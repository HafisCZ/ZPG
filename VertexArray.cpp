#include "VertexArray.h"

#include "Renderer.h"

VertexArray::VertexArray() {
	// generate vao, bind vertex array and its layout
	glGenVertexArrays(1, &m_rendererId);
	// bind vao object
	glBindVertexArray(m_rendererId);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &m_rendererId);
}

void VertexArray::addBuffer(const VertexBuffer & vb, const VertexBufferLayout & layout) {
	bind();
	vb.bind();

	const auto& elements = layout.getElements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];

		// enable vertex attrib array index
		glEnableVertexAttribArray(i);
		// set how opengl interprets buffer content
		// index of first attribute, amount of atributes, type, normalize data (to 0 - 1), size of vertex, byte offset of data in the same vertex
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*) offset);

		// shift offset
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::bind() const {
	glBindVertexArray(m_rendererId);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}
