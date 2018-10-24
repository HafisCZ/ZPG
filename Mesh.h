#pragma once

#include <memory>
#include <array>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"

using TextureHandle = std::shared_ptr<Texture>;

class Mesh {
	public:
		struct Properties {
			unsigned int vertex_count;
			unsigned int indice_count;
		};

	private:
		std::unique_ptr<VertexArray> _vao;
		std::unique_ptr<VertexBuffer> _vbo;
		std::unique_ptr<IndexBuffer> _ibo;
		
		Properties _properties;

		std::array<TextureHandle, 4> _textures;

	public:
		Mesh(void* vertices_ptr, void* indices_ptr, Properties properties, VertexBufferLayout vbl, TextureHandle diffuse = nullptr, TextureHandle specular = nullptr, TextureHandle normal = nullptr, TextureHandle height = nullptr);

		inline std::unique_ptr<VertexArray>& getVAO() { return _vao; }
		inline std::unique_ptr<VertexBuffer>& getVBO() { return _vbo; }
		inline std::unique_ptr<IndexBuffer>& getIBO() { return _ibo; }

		inline std::array<TextureHandle, 4>& getTextures() { return _textures; }

		inline const bool hasIBO() const { return _ibo != nullptr; }
		inline const Properties& getProperties() const { return _properties; }
};