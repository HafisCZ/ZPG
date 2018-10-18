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
		template <typename T, typename ... Args, typename ... Trgs> 
		Mesh(T loader(std::unique_ptr<VertexArray>&, std::unique_ptr<VertexBuffer>&, std::unique_ptr<IndexBuffer>&, std::unordered_map<TextureType, std::shared_ptr<Texture>>&, unsigned int&, Args ...), Trgs ... args)
		{
			std::function<T(std::unique_ptr<VertexArray>&, std::unique_ptr<VertexBuffer>&, std::unique_ptr<IndexBuffer>&, std::unordered_map<TextureType, std::shared_ptr<Texture>>&, unsigned int&, Args ...)> internal(loader);
			internal(m_vao, m_vbo, m_ibo, m_txt, m_vertex_count, args ...);
		}

		void draw(const Renderer& renderer, Program& program, bool no_uniforms = false);

		static void assimp(
			std::unique_ptr<VertexArray>& vao, std::unique_ptr<VertexBuffer>& vbo, std::unique_ptr<IndexBuffer>& ibo, std::unordered_map<TextureType, std::shared_ptr<Texture>>& txt, unsigned int& vc,
			std::vector<pnttb_t>& vertices, std::vector<unsigned int>& indices, std::unordered_map<TextureType, std::shared_ptr<Texture>>& textures
		);
};