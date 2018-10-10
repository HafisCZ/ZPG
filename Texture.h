#pragma once

#include <GL/glew.h>

#include <string>

class Texture {
	private:
		unsigned int m_handle;

		int m_width;
		int m_height;
		int m_bpp;

	public:
		Texture(const std::string& filepath, unsigned int mode = GL_CLAMP_TO_EDGE);

		~Texture();

		void bind(unsigned int slot = 0) const;
		void unbind() const;

		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }
};