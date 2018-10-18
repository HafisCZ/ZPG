#pragma once

#include <functional>

#include "Renderer.h"

enum TextureType {
	DIFFUSE_MAP, 
	SPECULAR_MAP, 
	NORMAL_MAP, 
	HEIGHT_MAP
};

class Mesh {
	private:
		std::unique_ptr<VertexBuffer> m_vbo;
		std::unique_ptr<VertexArray> m_vao;
		std::unique_ptr<IndexBuffer> m_ibo;
		std::unordered_map<TextureType, std::shared_ptr<Texture>> m_txt;

		unsigned int m_vertex_count;

	public:
		template <typename T, typename ... Args> using _mesh_gen_fun = std::function<T(std::unique_ptr<VertexArray>&, std::unique_ptr<VertexBuffer>&, std::unique_ptr<IndexBuffer>&, std::unordered_map<TextureType, std::shared_ptr<Texture>>&, unsigned int&, Args ...)>;
		template <typename ... Args> using _mesh_gen_fun_ptr = void(std::unique_ptr<VertexArray>&, std::unique_ptr<VertexBuffer>&, std::unique_ptr<IndexBuffer>&, std::unordered_map<TextureType, std::shared_ptr<Texture>>&, unsigned int&, Args ...);
		using _mesh_gen_fun_ptr_empty = _mesh_gen_fun_ptr<>;

		template <typename ... Args, typename ... Trgs> Mesh(_mesh_gen_fun_ptr<Args ...> loader, Trgs ... args) {
			_mesh_gen_fun<void, Args ... >internal(loader);
			internal(m_vao, m_vbo, m_ibo, m_txt, m_vertex_count, args ...);
		}

		void draw(const Renderer& renderer, Program& program, bool no_uniforms = false);
};