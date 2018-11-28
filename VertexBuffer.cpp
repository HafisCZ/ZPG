#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(void_cptr ptr, std::size_t size) : Buffer(VBO) {
	glGenBuffers(1, &_hid);
	glBindBuffer(GL_ARRAY_BUFFER, _hid);
	glBufferData(GL_ARRAY_BUFFER, size, ptr, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &_hid);
}

void VertexBuffer::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, _hid);
}

void VertexBuffer::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}