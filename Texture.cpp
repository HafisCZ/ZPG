#include "Texture.h"

#include <unordered_map>
#include <iostream>

#include "vendor/stb_image.h"

Texture::Texture(const std::string& filepath, unsigned int mode) {
	unsigned char* buffer = stbi_load(filepath.c_str(), &m_width, &m_height, &m_bpp, 0);

	if (buffer) {
		glGenTextures(1, &m_handle);
		glBindTexture(GL_TEXTURE_2D, m_handle);

		unsigned int format = GL_RGBA;
		if (m_bpp == 1) format = GL_RED;
		else if (m_bpp == 3) format = GL_RGB;
		else if (m_bpp == 4) format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, buffer);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
	}

	static unsigned int id = 1;
	m_id = id++;

	setSlot(0);
}

Texture::~Texture() {
	glDeleteTextures(1, &m_handle);
}

void Texture::bind(unsigned int slot) {
	if (slot == 0) {
		return;
	}

	if (setSlot(slot)) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_handle);
	}
}

void Texture::unbind() {
	setSlot(-1);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture::setSlot(unsigned int slot) {
	static std::unordered_map<unsigned int, unsigned int> binds;

	if (slot >= 0) {
		unsigned int sid;
		if ((sid = binds[slot]) != m_id) {
			binds[slot] = m_id;
			return true;
		}
	} else {
		binds[slot] = 0;
	}

	return false;
}

std::unordered_map<int, std::unique_ptr<Texture>> Texture::m_cache;
std::unordered_map<int, std::string> Texture::m_filepath_cache;