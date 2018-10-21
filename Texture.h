#pragma once

#include <GL/glew.h>

#include <memory>
#include <vector>
#include <string>

class Texture {
	private:
		unsigned int m_handle;
		unsigned int m_type;

	public:
		Texture(const std::string& filepath, unsigned int mode);
		Texture(const std::vector<std::string>& filepaths, unsigned int mode = GL_CLAMP_TO_EDGE);
		~Texture();

		static std::shared_ptr<Texture> load(const std::string& filepath, unsigned int mode = GL_CLAMP_TO_EDGE);

		void bind(unsigned int slot = 0) const;
		void unbind() const;
};