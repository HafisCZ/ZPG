#include "GBuffer.h"

#include "Buffer.h"

GBuffer::GBuffer(unsigned int width, unsigned int height) {
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);

	glGenTextures(Textures::length, _textures);

	for (unsigned int i = 0; i < Textures::length; i++) {
		glBindTexture(GL_TEXTURE_2D, _textures[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, (i == Textures::TEXTURE ? GL_RGBA : GL_RGB16F), width, height, 0, (i == Textures::TEXTURE ? GL_RGBA : GL_RGB), (i == Textures::TEXTURE ? GL_UNSIGNED_BYTE : GL_FLOAT), nullptr);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _textures[i], 0);
	}

	unsigned int* draw_buffers = new unsigned int[Textures::length];
	for (unsigned int i = 0; i < Textures::length; i++) {
		draw_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	glDrawBuffers(Textures::length, draw_buffers);

	glGenRenderbuffers(1, &_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, _depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth);

	unsigned int status;
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
		printf_s("GBUFFER ERROR: 0x%x\n", status);
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void GBuffer::bindTextures(unsigned int slot) {
	for (unsigned int i = 0; i < Textures::length; i++) {
		glActiveTexture(GL_TEXTURE0 + slot + i);
		glBindTexture(GL_TEXTURE_2D, _textures[i]);
	}
}

void GBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
}

void GBuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::copy(unsigned int width, unsigned int height) {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}