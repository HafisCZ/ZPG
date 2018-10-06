#pragma once

#include <glm/glm.hpp>

#include "Renderer.h"

struct ModelVertex {
	float x, y, z, u, v, o, p, q;

	bool operator == (const ModelVertex& mv) const {
		return (
			(x == mv.x && y == mv.y && z == mv.z) &&
			(u == mv.u && v == mv.v) &&
			(o == mv.o && p == mv.p && q == mv.q)
		);
	}
};

class Model {
	private:
		std::unique_ptr<VertexBuffer> m_vbo;
		std::unique_ptr<VertexArray> m_vao;
		std::unique_ptr<IndexBuffer> m_ibo;
	public:
		Model(const std::string& modelPath);

		void draw(Renderer renderer, Shader shader);

		static void loadModel(const std::string& filepath, std::vector<ModelVertex>& vertices, std::vector<unsigned int>& indices);
};