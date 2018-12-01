#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(void_cptr data, unsigned int count) : Buffer(IBO), _count(count) {
	glGenBuffers(1, &_hid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _hid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
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