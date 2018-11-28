#include "VertexArray.h"

std::size_t VertexLayout::getSize(std::size_t type) {
	switch (type) {
		case GL_UNSIGNED_INT:
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		default: return 0;
	}
}

VertexLayout VertexLayout::getPNTTB() {
	VertexLayout layout;
	layout.push<float>(3);
	layout.push<float>(3);
	layout.push<float>(2);
	layout.push<float>(3);
	layout.push<float>(3);
	return layout;
}

VertexLayout VertexLayout::getPNT() {
	VertexLayout layout;
	layout.push<float>(3);
	layout.push<float>(3);
	layout.push<float>(2);
	return layout;
}

VertexLayout VertexLayout::getPN() {
	VertexLayout layout;
	layout.push<float>(3);
	layout.push<float>(3);
	return layout;
}

VertexLayout VertexLayout::getPT() {
	VertexLayout layout;
	layout.push<float>(3);
	layout.push<float>(2);
	return layout;
}

VertexLayout VertexLayout::getP() {
	VertexLayout layout;
	layout.push<float>(3);
	return layout;
}

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

void VertexArray::addBuffer(VertexBuffer& vbo, VertexLayout vbl) {
	BufferGuard::attemptBind(this);
	BufferGuard::attemptBind(&vbo);

	const auto& elements = vbl.getElements();
	std::size_t offset = 0;
	for (int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normal, vbl.getStride(), (void_cptr)offset);

		offset += element.count * VertexLayout::getSize(element.type);
	}
}
