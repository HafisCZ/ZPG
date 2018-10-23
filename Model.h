#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <functional>

#include "Mesh.h"

class Model {
	private:
		std::vector<std::unique_ptr<Mesh>> m_meshes;

	public:
		Model() {}
		Model(void* vertices, unsigned int vertice_count, VertexBufferLayout vertex_layout) {
			std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(vertices, (void*) nullptr, vertex_layout, MeshDetails { vertice_count, 0 });
			m_meshes.push_back(std::move(mesh));
		}

		void addMesh(std::unique_ptr<Mesh>& mesh) { m_meshes.push_back(std::move(mesh)); }

		inline std::vector<std::unique_ptr<Mesh>>& getMeshes() { return m_meshes; }

		static Model load(const std::string& filepath);
};