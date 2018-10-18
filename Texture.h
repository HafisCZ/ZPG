#pragma once

#include <GL/glew.h>

#include <string>
#include <unordered_map>

class Texture {
	private:
		unsigned int m_handle;

		int m_width;
		int m_height;
		int m_bpp;

		unsigned int m_id;

		static std::unordered_map<int, std::unique_ptr<Texture>> m_cache;
		static std::unordered_map<int, std::string> m_filepath_cache;

	public:
		Texture(const std::string& filepath, unsigned int mode = GL_CLAMP_TO_EDGE);

		~Texture();

		void bind(unsigned int slot = 0);
		void unbind();

		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }

		bool setSlot(unsigned int slot);

		static int preload(const std::string& filepath, unsigned int mode = GL_CLAMP_TO_EDGE) {
			for (auto& entry : m_filepath_cache) {
				if (entry.second.compare(filepath) == 0) {
					return entry.first;
				}
			}

			m_cache[m_cache.size()] = std::make_unique<Texture>(filepath, mode);
			m_filepath_cache[m_cache.size() - 1] = filepath;
			
			return m_cache.size() - 1;
		}

		static Texture& repo(int id) {
			return *m_cache[id];
		}
};