#include "TerrainGenerator.h"

#include <glm/gtc/matrix_transform.hpp>

TerrainMesh::TerrainMesh(Program& program, std::function<void(std::unique_ptr<VertexArray>&, std::unique_ptr<VertexBuffer>&, std::unique_ptr<IndexBuffer>&, std::vector<TextureData>&, std::size_t&)> generator) : m_program(&program) {
	m_model = std::make_unique<Model>(Model::generator<>, generateData);
}

void TerrainMesh::generateData(std::unique_ptr<VertexArray>& vao, std::unique_ptr<VertexBuffer>& vbo, std::unique_ptr<IndexBuffer>& ibo, std::vector<TextureData>& txt, std::size_t& vc) {
	YGenerator generator(10.0f);

	std::vector<Vertex> vertices;
	vertices.reserve(64 * 64);

	std::vector<Indice> indices;
	indices.reserve(6 * (64 - 1) * (64 - 1));

	Vertex raws[64][64];

	for (unsigned int i = 0; i < 64; i++) {
		for (unsigned int j = 0; j < 64; j++) {
			raws[i][j] = { { i, generator.generate(i, j), j } };
		}
	}

	const glm::vec2 textures[4] = { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } };

	Vertex temp[4];
	unsigned int last;

	for (unsigned int i = 0; i < 64 - 1; i++) {
		for (unsigned int j = 0; j < 64 - 1; j++) {
			temp[0] = raws[i + 1][j];
			temp[1] = raws[i][j];
			temp[2] = raws[i][j + 1];
			temp[3] = raws[i + 1][j + 1];

			glm::vec3 normal = glm::normalize(glm::cross(temp[2].position, temp[1].position));

			last = vertices.size();
			for (unsigned int k = 0; k < 4; k++) {
				vertices.push_back({ temp[k].position, normal, textures[k] });
			}

			indices.push_back(last + 0);
			indices.push_back(last + 1);
			indices.push_back(last + 2);

			indices.push_back(last + 0);
			indices.push_back(last + 2);
			indices.push_back(last + 3);
		}
	}

	vertices.shrink_to_fit();
	indices.shrink_to_fit();

	vc = vertices.size();
	vao = std::make_unique<VertexArray>();
	vbo = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(Vertex));
	ibo = std::make_unique<IndexBuffer>(indices.data(), indices.size());
	vao->addBuffer(*vbo, VertexBufferLayout::DEFAULT_PNT());
}

void TerrainMesh::draw(const Renderer& renderer) {
	m_program->bind();

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f));
	m_program->setUniform("u_model", model);

	m_model->draw(renderer, *m_program);
}