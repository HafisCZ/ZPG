#pragma once

#include <sstream>
#include <fstream>
#include <regex>
#include <functional>

#include <iostream>

#include "Renderer.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture;

	bool operator == (const Vertex& vertex) const {
		return ( position == vertex.position && normal == vertex.normal &&	texture == vertex.texture );
	}
};

using Indice = unsigned int;

class Model {
	private:
		std::unique_ptr<VertexBuffer> m_vbo;
		std::unique_ptr<VertexArray> m_vao;
		std::unique_ptr<IndexBuffer> m_ibo;

		std::size_t m_vertex_count;

	public:
		template <typename T, typename ... Args, typename ... Trgs> Model(T loader(std::unique_ptr<VertexBuffer>&, std::unique_ptr<IndexBuffer>&, std::size_t& vc, Args ...), Trgs ... args) {
			m_vao = std::make_unique<VertexArray>();

			std::function<T(std::unique_ptr<VertexBuffer>&, std::unique_ptr<IndexBuffer>&, std::size_t&, Args ...)> internal(loader);
			internal(m_vbo, m_ibo, m_vertex_count, args ...);

			VertexBufferLayout vbl;
			vbl.push<float>(3);
			vbl.push<float>(3);
			vbl.push<float>(2);

			m_vao->addBuffer(*m_vbo, vbl);
		}

		void draw(const Renderer& renderer, const Program& program);
};

class ModelLoader {
	public:
		static void loadFromFile(std::unique_ptr<VertexBuffer>& vbo, std::unique_ptr<IndexBuffer>& ibo, std::size_t& vc, const std::string& filepath);
		static void loadFromArray(std::unique_ptr<VertexBuffer>& vbo, std::unique_ptr<IndexBuffer>& ibo, std::size_t& vc, Vertex* vertices, std::size_t size);
};