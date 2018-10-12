#pragma once

#include <GL/glew.h>

#include <string>

class Texture {
	private:
		unsigned int m_handle;

		int m_width;
		int m_height;
		int m_bpp;

		unsigned int m_id;

	public:
		Texture(const std::string& filepath, unsigned int mode = GL_CLAMP_TO_EDGE);

		~Texture();

		void bind(unsigned int slot = 0);
		void unbind();

		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }

		bool setSlot(unsigned int slot);
};