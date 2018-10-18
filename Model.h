#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class Model {
	private:
		std::vector<Mesh> m_meshes;

	public:
		template <typename T, typename ... Args, typename ... Trgs> Model(T loader(std::vector<Mesh>&, Args ...), Trgs ... args) {
			std::function<T(std::vector<Mesh>&, Args ...)> internal(loader);
			internal(m_meshes, args ...);
		}

		void draw(const Renderer& renderer, Program& program);

		static void assimp(std::vector<Mesh>& meshes, const std::string& filepath);

		template<typename T = void, typename ... Args> static void generator(std::vector<Mesh>& meshes, T gen(std::unique_ptr<VertexArray>&, std::unique_ptr<VertexBuffer>&, std::unique_ptr<IndexBuffer>&, std::vector<TextureData>&, std::size_t&, Args ...)) {
			meshes.emplace_back(gen);
		}

	private:
		static void _assimp_process_node(std::vector<Mesh>& meshes, aiNode* node, const aiScene* scene, const std::string& dir);
		static Mesh _assimp_process_mesh(aiMesh* mesh, const aiScene* scene, const std::string& dir);
		static aiTextureType _assimp_texture_type(TextureType type);
		static void _assimp_load_texture(std::vector<TextureData>& textures, aiMaterial* mat, TextureType type, const std::string& dir);
};