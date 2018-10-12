#pragma once

#include <GL/glew.h>

enum FramebufferTexture : unsigned int {
	TEXTURE_2D = GL_TEXTURE_2D,
	TEXTURE_3D = GL_TEXTURE_CUBE_MAP
};

class Framebuffer {
	private:
		unsigned int m_handle;
		unsigned int m_texture_handle;

		unsigned int m_width;
		unsigned int m_height;

		unsigned int m_type;
	public:
		Framebuffer(FramebufferTexture tex, unsigned int width, unsigned int height);

		void begin() const;
		void end(unsigned int width, unsigned int height) const;
};