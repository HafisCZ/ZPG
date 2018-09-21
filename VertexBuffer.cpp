#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
	// generate 1 buffer and save the id into buffer variable
	glGenBuffers(1, &m_rendererId);
	// select buffer, as array of data
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
	// load data into buffer, length of data to be loaded, source of data, 
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
	// delete buffer
	glDeleteBuffers(1, &m_rendererId);
}

void VertexBuffer::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
}

void VertexBuffer::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}