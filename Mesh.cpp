#include "Mesh.h"

Mesh::Mesh(void* vertices_ptr, void* indices_ptr, Properties properties, VertexBufferLayout vbl, TextureHandle diffuse, TextureHandle specular, TextureHandle normal, TextureHandle height)
	: _properties(properties), _textures{ { diffuse, specular, normal, height } }
{
	_vao = std::make_unique<VertexArray>();
	_vbo = std::make_unique<VertexBuffer>(vertices_ptr, properties.vertex_count * vbl.getStride());
	_vao->addBuffer(*_vbo, vbl);

	if (indices_ptr) {
		_ibo = std::make_unique<IndexBuffer>(indices_ptr, properties.indice_count);
	}
}