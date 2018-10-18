#include "TerrainGenerator.h"

#include <glm/gtc/matrix_transform.hpp>

#include "YGenerator.h"

void Terrain::generateData(std::unique_ptr<VertexArray>& vao, std::unique_ptr<VertexBuffer>& vbo, std::unique_ptr<IndexBuffer>& ibo, std::unordered_map<TextureType, std::shared_ptr<Texture>>& txt, unsigned int& vc) {
	YGenerator generator(10.0f);

	std::vector<pnt_t> vertices;
	vertices.reserve(64 * 64);

	std::vector<unsigned int> indices;
	indices.reserve(6 * (64 - 1) * (64 - 1));

	pnt_t raws[64][64];

	for (unsigned int i = 0; i < 64; i++) {
		for (unsigned int j = 0; j < 64; j++) {
			raws[i][j] = { { i, generator.generate(i, j), j } };
		}
	}

	const glm::vec2 textures[4] = { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } };

	pnt_t temp[4];
	unsigned int last;

	for (unsigned int i = 0; i < 64 - 1; i++) {
		for (unsigned int j = 0; j < 64 - 1; j++) {
			temp[0] = raws[i + 1][j];
			temp[1] = raws[i][j];
			temp[2] = raws[i][j + 1];
			temp[3] = raws[i + 1][j + 1];

			glm::vec3 normal = glm::normalize(glm::cross(temp[2].pos, temp[1].pos));

			last = vertices.size();
			for (unsigned int k = 0; k < 4; k++) {
				vertices.push_back({ temp[k].pos, normal, textures[k] });
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
	vbo = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(pnt_t));
	ibo = std::make_unique<IndexBuffer>(indices.data(), indices.size());
	vao->addBuffer(*vbo, VertexBufferLayout::DEFAULT_PNT());
}