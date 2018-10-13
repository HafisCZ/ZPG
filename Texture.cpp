#include "Texture.h"

#include <unordered_map>
#include <iostream>

#include "vendor/stb_image.h"

Texture::Texture(const std::string& filepath, unsigned int mode) {
	stbi_set_flip_vertically_on_load(1);

	unsigned char* buffer = stbi_load(filepath.c_str(), &m_width, &m_height, &m_bpp, 4);

	glGenTextures(1, &m_handle);
	glBindTexture(GL_TEXTURE_2D, m_handle);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);

	static unsigned int id = 1;
	m_id = id++;
	setSlot(0);

	std::cout << "[TEXTURE] ID: " << m_id << std::endl;
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
