#include "Framebuffer.h"

Framebuffer::Framebuffer(std::size_t size) : _size(size), Buffer(FRAME) {
	glGenFramebuffers(1, &_hid);
}

void Framebuffer::bind() {
	glViewport(0, 0, _size, _size);

	glBindFramebuffer(GL_FRAMEBUFFER, _hid);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::unbind(std::size_t width, std::size_t height) {
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
