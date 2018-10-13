#pragma once

#include <memory>

#include "Renderer.h"
#include "Program.h"

struct Vertex {
	glm::vec3 position;
	glm::vec2 texture;
	glm::vec3 normal;

	bool operator == (const Vertex& vertex) const {
		return (
			position == vertex.position &&
			normal == vertex.normal &&
			texture == vertex.texture
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

		void render(const Renderer& renderer, const Program& program);

		static void loadModel(const std::string& filepath, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
};