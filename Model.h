#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <functional>

#include "Mesh.h"

class Model {
	private:
		std::vector<Mesh> m_meshes;

	public:
		Model(std::vector<Mesh>& meshes) : m_meshes(std::move(meshes)) {}

		inline const std::vector<Mesh>& getMeshes() const { return m_meshes; }

		static Model load(const std::string& filepath);
};