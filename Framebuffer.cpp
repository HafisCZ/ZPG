#include "Framebuffer.h"

Framebuffer::Framebuffer(unsigned int width, unsigned int height) : m_width(width), m_height(height) {

}

void Framebuffer::bind() const {
	glViewport(0, 0, m_width, m_height);

	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::unbind(unsigned int width, unsigned int height) const {
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Framebuffer3D::Framebuffer3D(unsigned int size) : Framebuffer(size, size) {
	glGenFramebuffers(1, &m_handle);
	glGenTextures(1, &m_texture_handle);

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_handle);

	for (unsigned int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_texture_handle, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer3D::unbind(unsigned int slot, unsigned int width, unsigned int height) const {
	Framebuffer::unbind(width, height);

	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_handle);
}
