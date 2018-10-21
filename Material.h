#pragma once

#include "Texture.h"

enum MapType {
	DIFFUSE_MAP, SPECULAR_MAP, BUMP_NORMAL_MAP, HEIGHT_MAP
};

class Material {
	private:
		MapType m_type;
		std::shared_ptr<Texture> m_texture;

	public:
		Material(MapType type, const std::string& texture) : m_type(type), m_texture(Texture::load(texture)) {}

		inline MapType getType() const { return m_type; }
		inline const std::shared_ptr<Texture>& getTexture() const { return m_texture; }
};