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

		void draw(const Renderer& renderer, Program& program, bool no_uniforms = false);

		static void assimp(std::vector<Mesh>& meshes, const std::string& filepath);

		static void generator(std::vector<Mesh>& meshes, void gen(std::unique_ptr<VertexArray>&, std::unique_ptr<VertexBuffer>&, std::unique_ptr<IndexBuffer>&, std::unordered_map<TextureType, std::shared_ptr<Texture>>&, unsigned int&)) {
			meshes.emplace_back(gen); 
		}

	private:
		static void _assimp_process_node(std::vector<Mesh>& meshes, aiNode* node, const aiScene* scene, const std::string& dir);
		static Mesh _assimp_process_mesh(aiMesh* mesh, const aiScene* scene, const std::string& dir);
		static void _assimp_load_texture(std::unordered_map<TextureType, std::shared_ptr<Texture>>& textures, aiMaterial* mat, aiTextureType type, const std::string& dir);
};