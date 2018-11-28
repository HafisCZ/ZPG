#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(void_cptr ptr, std::size_t size) : Buffer(IBO), _indices(size) {
	glGenBuffers(1, &_hid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _hid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(std::size_t), ptr, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &_hid);
}

void IndexBuffer::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _hid);
}

void IndexBuffer::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}