#include "Mesh.h"

void Mesh::draw(const Renderer& renderer, Program& program) {
	unsigned int iter[4] = {
		1, 1, 1, 1
	};

	for (unsigned int i = 0; i < m_txt.size(); i++) {
		if (m_txt[i].type == DIFFUSE) {

			program.setUniform("texture_diffuse" + std::to_string(iter[0]++), i + 1);
		} else 	if (m_txt[i].type == SPECULAR) {

			program.setUniform("texture_specular" + std::to_string(iter[1]++), i + 1);
		} else 	if (m_txt[i].type == NORMAL) {

			program.setUniform("texture_normal" + std::to_string(iter[2]++), i + 1);
		} else 	if (m_txt[i].type == HEIGHT) {

			program.setUniform("texture_height" + std::to_string(iter[3]++), i + 1);
		}

		renderer.bindTexture(m_txt[i].id, i + 1);
	}

	if (m_ibo) {
		renderer.draw(*m_vao, *m_ibo, program);
	} else {
		renderer.draw(*m_vao, m_vertex_count, program);
	}
}

void Mesh::assimp(std::unique_ptr<VertexArray>& vao, std::unique_ptr<VertexBuffer>& vbo, std::unique_ptr<IndexBuffer>& ibo,
	std::vector<TextureData>& txt, std::size_t& vc, std::vector<pnttb_t>& vertices, std::vector<unsigned int>& indices, std::vector<TextureData>& textures) {
	vc = vertices.size();
	vao = std::make_unique<VertexArray>();
	vbo = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(pnttb_t));
	ibo = std::make_unique<IndexBuffer>(indices.data(), indices.size());
	vao->addBuffer(*vbo, VertexBufferLayout::DEFAULT_PNTTB());

	txt = std::move(textures);
}