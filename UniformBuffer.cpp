#include "UniformBuffer.h"

UniformLayout::UniformLayout() : _size(0), _istr(0) {
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &_step);
}

void UniformLayout::pack() {
	Block block = { _blocks.size(), _size, _istr };
	block.items.insert(block.items.end(), _items.begin(), _items.end());

	_blocks.push_back(block);
	_items.clear();

	_size += Alignment::alignto(_istr, _step);
	_istr = 0;
}

UniformBuffer::UniformBuffer(UniformLayout& layout) : Buffer(UNIFORM), _layout(layout) {
	glGenBuffers(1, &_hid);
	glBindBuffer(GL_UNIFORM_BUFFER, _hid);
	glBufferData(GL_UNIFORM_BUFFER, layout.getSize(), nullptr, GL_DYNAMIC_DRAW);

	for (const auto& elem : layout.getBlocks()) {
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