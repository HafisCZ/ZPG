#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(void_cptr data, unsigned int count, VertexLayout layout) : Buffer(VBO), _count(count), _layout(layout) {
	glGenBuffers(1, &_hid);
	glBindBuffer(GL_ARRAY_BUFFER, _hid);
	glBufferData(GL_ARRAY_BUFFER, count * layout.getStride(), data, GL_STATIC_DRAW);
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