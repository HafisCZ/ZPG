#include "Texture.h"

#include <stack>

TextureGuard::TextureGuard(unsigned int offset, unsigned int slots) {
	for (unsigned int i = 0; i < slots; i++) {
		_slotAlloc[i + offset] = nullptr;
	}
}

TextureGuard& TextureGuard::getInstance() {
	static TextureGuard instance(8, 32);
	return instance;
}

unsigned int TextureGuard::autoBind(Texture* texture) {
	return getInstance()._autoBind(texture);
}

void TextureGuard::flush() {
	getInstance()._flush();
}

void TextureGuard::_flush() {
	for (auto& it : _slotLock) {
		it.second = false;
	}
}

unsigned int TextureGuard::_autoBind(Texture* texture) {
	unsigned int selectedSlot = 0;
	unsigned long leastBound = -1;

	for (auto& it : _slotAlloc) {
		if (it.second == texture) {
			return it.first;
		}

		if (_slotLock[it.first]) {
			continue;
		}

		if (!it.second) {
			selectedSlot = it.first;
			break;
		}

		if (it.second->getBindCount() < leastBound) {
			selectedSlot = it.first;
			leastBound = it.second->getBindCount();
		}
	}

	_slotAlloc[selectedSlot] = texture;
	_slotLock[selectedSlot] = true;

	glActiveTexture(GL_TEXTURE0 + selectedSlot);
	texture->bindUnsafe(selectedSlot);

	return selectedSlot;
}

std::shared_ptr<Texture> TextureLoader::load(const std::string& filepath, unsigned int mode) {
	static std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

	if (textures.find(filepath) == textures.end()) {
		textures[filepath] = std::make_shared<Texture2D>(filepath, mode);
	}

	return textures[filepath];
}

std::shared_ptr<Texture> TextureLoader::load(const std::vector<std::string>& filepaths, unsigned int mode) {
	static std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

	std::string hash = filepaths[0].substr(0, filepaths[0].find_last_of('/') + 1);

	if (textures.find(hash) == textures.end()) {
		textures[hash] = std::make_shared<Texture3D>(filepaths, mode);
	}

	return textures[hash];
}