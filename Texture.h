#pragma once

#include <GL/glew.h>

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

#include "vendor/stb_image.h"


#include <iostream>

class Texture;

class TextureBindGuard {
	public:
		static void attemptBind(unsigned int slot, Texture* texture);
		static unsigned int autoBind(Texture* texture);
};

class Texture {
	protected:
		unsigned int _handle;
		unsigned int _type;
		unsigned long _binds;

		int _ppm;
		int _format;
		int _width;
		int _height;

	protected:
		bool readFile(unsigned char*& buffer, const std::string& filepath) {
			buffer = stbi_load(filepath.c_str(), &_width, &_height, &_ppm, 0);

			return buffer;
		}

		void cleanup(unsigned char* buffer) {
			if (buffer) {
				stbi_image_free(buffer);
			}
		}

		unsigned int getFormat() {
			switch (_ppm) {
				case 1: return GL_RED;
				case 3: return GL_RGB;
				default:
					return GL_RGBA;
			}
		}

	public:
		Texture(unsigned int type) : _type(type), _ppm(0), _format(0), _width(0), _height(0), _binds(0) {
			glGenTextures(1, &_handle);
		}

		virtual ~Texture() {
			glDeleteTextures(1, &_handle);
		}

		void bind(unsigned int slot = 0) {
			TextureBindGuard::attemptBind(slot, this);

			_binds++;
		}

		void bindUnsafe(unsigned int slot) const {
			glBindTexture(_type, _handle);
		}

		unsigned long getBindCount() {
			return _binds;
		}
};

class Texture2D : public Texture {
	public:
		Texture2D(const std::string& filepath, unsigned int mode = GL_CLAMP_TO_EDGE) : Texture(GL_TEXTURE_2D) {
			unsigned char* buffer;
			if (readFile(buffer, filepath)) {
				glBindTexture(_type, _handle);
				glTexImage2D(_type, 0, getFormat(), _width, _height, 0, getFormat(), GL_UNSIGNED_BYTE, buffer);

				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);

				cleanup(buffer);
			}
		}
};

class Texture3D : public Texture {
	public:
		Texture3D(const std::vector<std::string>& filepaths, unsigned int mode = GL_CLAMP_TO_EDGE) : Texture(GL_TEXTURE_CUBE_MAP) {
			glBindTexture(_type, _handle);

			unsigned char* buffer;
			for (unsigned int i = 0; i < filepaths.size(); i++) {
				if (readFile(buffer, filepaths[i])) {
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
					cleanup(buffer);
				}
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, mode);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, mode);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, mode);
		}
};

class TextureLoader {
	public:
		static std::shared_ptr<Texture> load(const std::string& filepath, unsigned int mode = GL_CLAMP_TO_BORDER);
		static std::shared_ptr<Texture> load(const std::vector<std::string>& filepaths, unsigned int mode = GL_CLAMP_TO_BORDER);
};