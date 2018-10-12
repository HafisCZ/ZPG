#include "UniformBuffer.h"

UniformBuffer::UniformBuffer(const UniformBufferLayout& layout) : m_layout(layout), Buffer(GL_UNIFORM_BUFFER) {
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

void UniformBuffer::bind() {
	if (setAsBound(true)) {
		glBindBuffer(GL_UNIFORM_BUFFER, m_handle);
	}
}

void UniformBuffer::unbind() {
	setAsBound(false);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}