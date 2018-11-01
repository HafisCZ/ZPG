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
	static std::stack<unsigned int> slotStack({ 8, 9, 10, 11, 12, 13, 14, 15 });

	static std::unordered_map<Texture*, unsigned int> boundTextures;

	if (slotStack.empty()) {
		// rebind texture over another 
	} else {
		if (boundTextures[texture] == 0) {
			boundTextures[texture] = slotStack.top();

			glActiveTexture(GL_TEXTURE0 + slotStack.top());
			texture->bindUnsafe(slotStack.top());

			slotStack.pop();
		}

		return boundTextures[texture];
	}
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