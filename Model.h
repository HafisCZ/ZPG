#pragma once

#include "Mesh.h"

class Model {
	private:
		std::vector<std::shared_ptr<Mesh>> _meshes;

	public:
		Model(const std::string& filepath);

		inline std::vector<std::shared_ptr<Mesh>>& getMeshes() { return _meshes; }

	private:
		void loadViaAssimp(const std::string& filepath);
};