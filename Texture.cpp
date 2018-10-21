#include "Texture.h"

#include <unordered_map>

#include "vendor/stb_image.h"

std::shared_ptr<Texture> Texture::load(const std::string& filepath, unsigned int mode) {
	static std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
	if (textures.find(filepath) == textures.end()) {
		textures[filepath] = std::make_shared<Texture>(filepath, mode);
	}

	return textures[filepath];
}

Texture::Texture(const std::string& filepath, unsigned int mode) : m_type(GL_TEXTURE_2D) {
	int width, height, channel_count;

	unsigned char* buffer = stbi_load(filepath.c_str(), &width, &height, &channel_count, 0);

	glGenTextures(1, &m_handle);
	glBindTexture(GL_TEXTURE_2D, m_handle);

	unsigned int format = channel_count == 1 ? GL_RED : (channel_count == 3 ? GL_RGB : GL_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, buffer);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);

	if (buffer) {
		stbi_image_free(buffer);
	}
}

Texture::Texture(const std::vector<std::string>& filepaths, unsigned int mode) : m_type(GL_TEXTURE_CUBE_MAP) {
	int width, height, channel_count;

	glGenTextures(1, &m_handle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle);

	for (unsigned int i = 0; i < filepaths.size(); i++) {
		unsigned char* buffer = stbi_load(filepaths[i].c_str(), &width, &height, &channel_count, 0);

		if (buffer) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);

			stbi_image_free(buffer);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, mode);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, mode);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, mode);
}

Texture::~Texture() {
	glDeleteTextures(1, &m_handle);
}

void Texture::bind(unsigned int slot) const {

	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(m_type, m_handle);
}

void Texture::unbind() const {
	glBindTexture(m_type, 0);
}