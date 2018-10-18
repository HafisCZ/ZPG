#pragma once

#include <functional>

#include "Renderer.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture;

	bool operator == (const Vertex& vertex) const {
		return (position == vertex.position && normal == vertex.normal &&	texture == vertex.texture);
	}
};

using Indice = unsigned int;

enum TextureType {
	DIFFUSE, SPECULAR, NORMAL, HEIGHT
};

struct TextureData {
	TextureType type;
	int id;
};

struct pnttb_t {
	glm::vec3 pos, nor;
	glm::vec2 tex;
	glm::vec3 tan, bta;
};

class Mesh {
	private:
	std::unique_ptr<VertexBuffer> m_vbo;
	std::unique_ptr<VertexArray> m_vao;
	std::unique_ptr<IndexBuffer> m_ibo;
	std::vector<TextureData> m_txt;

	std::size_t m_vertex_count;

	public:
		template <typename T, typename ... Args, typename ... Trgs> Mesh(T loader(std::unique_ptr<VertexArray>&, std::unique_ptr<VertexBuffer>&, std::unique_ptr<IndexBuffer>&, std::vector<TextureData>&, std::size_t&, Args ...), Trgs ... args) {
			std::function<T(std::unique_ptr<VertexArray>&, std::unique_ptr<VertexBuffer>&, std::unique_ptr<IndexBuffer>&, std::vector<TextureData>&, std::size_t&, Args ...)> internal(loader);
			internal(m_vao, m_vbo, m_ibo, m_txt, m_vertex_count, args ...);
		}

		void draw(const Renderer& renderer, Program& program);

		static void assimp(std::unique_ptr<VertexArray>& vao, std::unique_ptr<VertexBuffer>& vbo, std::unique_ptr<IndexBuffer>& ibo,
			std::vector<TextureData>& txt, std::size_t& vc, std::vector<pnttb_t>& vertices, std::vector<unsigned int>& indices, std::vector<TextureData>& textures);
};