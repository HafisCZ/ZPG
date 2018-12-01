#pragma once

#include <unordered_map>

#include "Texture.h"

class TextureManager {
	private:
		using TextureHandle = std::shared_ptr<Texture>;

		std::unordered_map<std::string, TextureHandle> _repository;

	public:
		static TextureManager& getManager() {
			static TextureManager manager;
			return manager;
		}

		TextureHandle querry(const std::string& filepath, unsigned int flag = 0) {
			if (!_repository[filepath]) {
				_repository[filepath] = std::make_unique<Texture>(filepath, flag);
			}

			return _repository[filepath];
		}
};