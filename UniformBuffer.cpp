#include "UniformBuffer.h"

UniformLayout::UniformLayout() : _size(0), _istr(0) {
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &_step);
}

void UniformLayout::pack() {
	UniformGroup ube = { _elements.size(), _size, _istr };
	ube.elements.insert(ube.elements.end(), _items.begin(), _items.end());

	_elements.push_back(ube);
	_items.clear();

	_size += Alignment::alignto(_istr, _step);
	_istr = 0;
}

UniformBuffer::UniformBuffer(const UniformLayout& layout) : Buffer(UNIFORM), _layout(layout) {
	glGenBuffers(1, &_hid);
	glBindBuffer(GL_UNIFORM_BUFFER, _hid);
	glBufferData(GL_UNIFORM_BUFFER, layout.getSize(), nullptr, GL_DYNAMIC_DRAW);

	for (const auto& elem : layout.getElements()) {
		glBindBufferRange(GL_UNIFORM_BUFFER, elem.index, _hid, elem.stride, elem.size);
	}
}

UniformBuffer::~UniformBuffer() {
	glDeleteBuffers(1, &_hid);
}

void UniformBuffer::bind() {
	glBindBuffer(GL_UNIFORM_BUFFER, _hid);
}

void UniformBuffer::unbind() {
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
