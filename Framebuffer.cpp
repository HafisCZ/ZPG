#include "Framebuffer.h"

Framebuffer::Framebuffer(FramebufferTexture tex, unsigned int width, unsigned int height)
	: m_width(width), m_height(height), m_type(tex)
{
	glGenFramebuffers(1, &m_handle);
	glGenTextures(1, &m_texture_handle);

	if (tex == TEXTURE_3D) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_handle);

		for (unsigned int i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_texture_handle, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}

void Framebuffer::begin() const {
	glViewport(0, 0, m_width, m_height);

	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::end(unsigned int width, unsigned int height) const {
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(m_type, m_texture_handle);
}