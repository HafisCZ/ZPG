#include "Texture.h"

#include <stack>

void TextureBindGuard::attemptBind(unsigned int slot, Texture* texture) {
	static std::unordered_map<Texture*, unsigned int> boundTextures;
	static std::size_t activeSlot = -1;

	if (activeSlot != slot) {
		glActiveTexture(GL_TEXTURE0 + slot);
		activeSlot = slot;
	}

	if (boundTextures[texture] != slot) {
		boundTextures[texture] = slot;
		texture->bindUnsafe(slot);
	}
}

unsigned int TextureBindGuard::autoBind(Texture* texture) {
	static std::unordered_map<unsigned int, Texture*> slotAlloc;
	static bool slotAllocInit = [] { 
		for (unsigned int i = 0; i < 16; i++) {
			slotAlloc.emplace(8 + i, nullptr);
		}
		return true; 
	}();

	unsigned int slot = 0;
	unsigned long least = -1;

	for (auto& keyval : slotAlloc) {
		if (keyval.second == texture) {
			return keyval.first;
		} else if (keyval.second == nullptr) {
			slot = keyval.first;
			break;
		} else if (keyval.second->getBindCount() < least) {
			slot = keyval.first;
			least = keyval.second->getBindCount();
		}
	}

	slotAlloc[slot] = texture;

	glActiveTexture(GL_TEXTURE0 + slot);
	texture->bindUnsafe(slot);

	return slot;
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