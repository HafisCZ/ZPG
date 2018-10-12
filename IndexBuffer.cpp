#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const void* data, unsigned int count)
	: m_count(count), Buffer(GL_ELEMENT_ARRAY_BUFFER)
{
	glGenBuffers(1, &m_handle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &m_handle);
}

void IndexBuffer::bind() {
	if (setAsBound(true)) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
	}
}

void IndexBuffer::unbind() {
	setAsBound(false);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}