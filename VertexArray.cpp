#include "VertexArray.h"

VertexArray::VertexArray() : Buffer(VAO) {
	glGenVertexArrays(1, &_hid);
	glBindVertexArray(_hid);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &_hid);
}


void VertexArray::bind() {
	glBindVertexArray(_hid);
}

void VertexArray::unbind() {
	glBindVertexArray(0);
}

void VertexArray::setBuffer(VertexBuffer& vbo) {
	BufferGuard::attemptBindEx(*this, vbo);

	const auto& blocks = vbo.getLayout().getBlocks();
	unsigned int blockOffset = 0;

	for (unsigned int i = 0; i < blocks.size(); i++) {
		const auto& block = blocks[i];

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, block.count, block.type, block.normalized ? GL_TRUE : GL_FALSE, vbo.getLayout().getStride(), (void_cptr)blockOffset);

		blockOffset += block.size;
	}

}