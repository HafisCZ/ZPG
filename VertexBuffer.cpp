#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) : Buffer(GL_ARRAY_BUFFER) {
	glGenBuffers(1, &m_handle);
	glBindBuffer(GL_ARRAY_BUFFER, m_handle);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &m_handle);
}

void VertexBuffer::bind() {
	if (setAsBound(true)) {
		glBindBuffer(GL_ARRAY_BUFFER, m_handle);
	}
}

void VertexBuffer::unbind() {
	setAsBound(false);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}