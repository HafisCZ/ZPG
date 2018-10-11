#pragma once

#include <unordered_map>

#include "Texture.h"

class TextureCache {
	private:
		std::unordered_map<unsigned int, const Texture*> m_map;

	public:
		void bind(const Texture& texture, unsigned int slot = 0) {
			if (m_map[slot] != &texture) {
				m_map[slot] = &texture;

				texture.bind(slot);
			}
		}
};