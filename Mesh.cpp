#include "Mesh.h"





/*
void Mesh::draw(const Renderer& renderer, Program& program, bool no_uniforms) {
	if (!no_uniforms) {
		program.bind();

		bool has_diffuse = m_txt.count(DIFFUSE_MAP) > 0;
		bool has_specular = m_txt.count(SPECULAR_MAP) > 0;
		bool has_normal = m_txt.count(NORMAL_MAP) > 0;
		bool has_height = m_txt.count(HEIGHT_MAP) > 0;

		program.setUniform("texture_diffuse_enable", has_diffuse);
		program.setUniform("texture_specular_enable", has_specular);
		program.setUniform("texture_normal_enable", has_normal);
		program.setUniform("texture_height_enable", has_height);

		if (has_diffuse) {
			program.setUniform("texture_diffuse", 8);
			m_txt[DIFFUSE_MAP]->bind(8);
		}

		if (has_specular) {
			program.setUniform("texture_specular", 9);
			m_txt[SPECULAR_MAP]->bind(9);
		} else {
			program.setUniform("texture_specular", 8);
		}

		if (has_normal) {
			program.setUniform("texture_normal", 10);
			m_txt[NORMAL_MAP]->bind(10);
		} else {
			program.setUniform("texture_normal", 8);
		}

		if (has_height) {
			program.setUniform("texture_height", 11);
			m_txt[HEIGHT_MAP]->bind(11);
		} else {
			program.setUniform("texture_height", 8);
		}
	}

	if (m_ibo) {
		renderer.draw(*m_vao, *m_ibo, program);
	} else {
		renderer.draw(*m_vao, m_vertex_count, program);
	}
}*/