#pragma once

#include <functional>

#include "Mesh.h"

class Model {
	private:
		std::vector<std::shared_ptr<Mesh>> _meshes;

	public:
		Model(const std::string& filepath);

		template <typename T, typename ... Args, typename = typename std::enable_if<std::is_same<T, void*>::value>::type>  Model(T t, Args ... args) : _meshes{ { std::forward<Args>(args)... } } {}

		inline std::vector<std::shared_ptr<Mesh>>& getMeshes() { return _meshes; }

	private:
		void loadViaAssimp(const std::string& filepath);
};