#include "Texture.h"

#include "vendor/stb_image.h"

Texture::Texture(const std::string& filepath)
	: m_rendererId(0), m_filePath(filepath), m_localBuffer(nullptr), m_width(0), m_height(0), m_bpp(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_localBuffer = stbi_load(filepath.c_str(), &m_width, &m_height, &m_bpp, 4);

	// generate texture
	glGenTextures(1, &m_rendererId);
	// bind texture
	glBindTexture(GL_TEXTURE_2D, m_rendererId);

	// set minification filtering - linear resample
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// set magnification filtering - linear sample
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// clamps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// send data to opengl - text type, depth, format, width, height, 0px border around, data format, data type, source
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_localBuffer) {
		stbi_image_free(m_localBuffer);
	}
}

Texture::~Texture() {
	glDeleteTextures(1, &m_rendererId);
}

void Texture::bind(unsigned int slot) const {
	// set active slot
	glActiveTexture(GL_TEXTURE0 + slot);
	// bind texture
	glBindTexture(GL_TEXTURE_2D, m_rendererId);
}

void Texture::unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}