#include "UniformBuffer.h"

UniformBuffer::UniformBuffer(const UniformBufferLayout& layout) : m_layout(layout) {
	glGenBuffers(1, &m_handle);
	glBindBuffer(GL_UNIFORM_BUFFER, m_handle);
	glBufferData(GL_UNIFORM_BUFFER, layout.getSize(), nullptr, GL_DYNAMIC_DRAW);

	for (const auto& elem : layout.getElements()) {
		glBindBufferRange(GL_UNIFORM_BUFFER, elem.index, m_handle, elem.stride, elem.size);
	}
}

UniformBuffer::~UniformBuffer() {
	glDeleteBuffers(1, &m_handle);
}

void UniformBuffer::setUniformBlock(unsigned int index, const void* a) {
	glBufferSubData(GL_UNIFORM_BUFFER, m_layout.getElements()[index].blockStride(), m_layout.getElements()[index].blockSize(), a);
}

void UniformBuffer::setUniformBlock(unsigned int index, unsigned int position, const void* a) {
	glBufferSubData(GL_UNIFORM_BUFFER, m_layout.getElements()[index].blockStride(position), m_layout.getElements()[index].itemSize(position), a);
}

void UniformBuffer::bind() const {
	glBindBuffer(GL_UNIFORM_BUFFER, m_handle);
}

void UniformBuffer::unbind() const {
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}