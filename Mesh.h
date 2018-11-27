#pragma once

#include <memory>
#include <array>
#include <unordered_map>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"

using TextureHandle = std::shared_ptr<Texture>;

class MeshTexturePack {
	public:
		enum class Type {
			DIFFUSE, NORMAL, HEIGHT, SPECULAR
		};

	private:
		std::unordered_map<Type, TextureHandle> _textures;

	public:
		inline TextureHandle requestHandleOfType(Type type) {
			return _textures[type];
		}

		void setHandleOfType(Type type, TextureHandle handle) {
			_textures[type] = handle;
		}
};

class Mesh {
	public:
		struct Property {
			std::size_t vertex_cnt;
			std::size_t indice_cnt;
		};

	private:
		VertexArray _vao;
		VertexBuffer _vbo;
		IndexBuffer _ibo;
		Property _prop;
		MeshTexturePack _pack;

	public:
		Mesh(void* vptr, void* iptr, Property prop, VertexBufferLayout lay, MeshTexturePack pack) : _prop(prop), _pack(pack), _vao(), _vbo(vptr, prop.vertex_cnt * lay.getStride()), _ibo(iptr, prop.indice_cnt) {
			_vao.addBuffer(_vbo, lay);
		}

		inline const bool hasIndices() const { return _prop.indice_cnt; }

		inline const VertexArray& getVAO() { return _vao; }
		inline const VertexBuffer& getVBO() { return _vbo; }
		inline const IndexBuffer& getIBO() { return _ibo; }
		inline const Property& getProperty() { return _prop; }
		inline MeshTexturePack& getTexturePack() { return _pack; }
};