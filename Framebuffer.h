#pragma once

#include <GL/glew.h>

class Framebuffer {
	protected:
		unsigned int m_handle;
		unsigned int m_texture_handle;

		unsigned int m_width;
		unsigned int m_height;

	public:
		Framebuffer(unsigned int width, unsigned int height);

		virtual void bind() const;
		virtual void unbind(unsigned int width, unsigned int height) const;
};

class Framebuffer3D : public Framebuffer {
	public:
		Framebuffer3D(unsigned int size);

		void unbind(unsigned int slot, unsigned int width, unsigned int size) const;
};