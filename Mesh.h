#pragma once

#include <memory>
#include <array>
#include <unordered_map>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"

using TextureHandle = std::shared_ptr<Texture>;

namespace MeshData {

	enum class Texture {
		DIFFUSE, NORMAL, HEIGHT, SPECULAR
	};

	class Pack {
		private:
			std::unordered_map<Texture, TextureHandle> _textures;

		public:
			inline TextureHandle getHandle(Texture type) { return _textures[type]; }
			void setHandle(Texture type, TextureHandle handle) { _textures[type] = handle; }
	};

	struct Property {
		std::size_t vertex_cnt;
		std::size_t indice_cnt;
	};

}

class Mesh {
	private:
		VertexArray _vao;
		VertexBuffer _vbo;
		IndexBuffer _ibo;
		MeshData::Pack _pack;

	public:
		Mesh(void* vptr, void* iptr, VertexLayout layout, MeshData::Property prop, MeshData::Pack pack) : _pack(pack), _vao(), _vbo(vptr, prop.vertex_cnt, layout), _ibo((_vao.setBuffer(_vbo), iptr), prop.indice_cnt) { }

		inline const bool hasIndices() { return _ibo.getCount(); }

		inline VertexArray& getVAO() { return _vao; }
		inline VertexBuffer& getVBO() { return _vbo; }
		inline IndexBuffer& getIBO() { return _ibo; }
		inline MeshData::Pack& getTexturePack() { return _pack; }
};