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
		void addMesh(std::unique_ptr<Mesh>& mesh) { m_meshes.push_back(std::move(mesh)); }

		inline std::vector<std::unique_ptr<Mesh>>& getMeshes() { return m_meshes; }

		static Model load(const std::string& filepath);
};