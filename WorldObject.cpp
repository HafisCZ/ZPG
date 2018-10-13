#include "WorldObject.h"

#include <glm/gtc/matrix_transform.hpp>

WorldObject::WorldObject(Model& model, Program& program, unsigned int texslot, unsigned int spcslot) : m_model(&model), m_program(&program), m_position(0.0f), m_transformation(1.0f), m_texslot(texslot), m_spcslot(spcslot) {
	static unsigned int id = 0;
	m_id = id++;
}

void WorldObject::render(Renderer& renderer) {
	m_program->bind();

	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_position) * m_transformation;
	m_program->uniform4mat("u_model", model);

	if (!(m_spcslot == -1 || m_texslot == -1)) {
		glm::mat4 inver = glm::transpose(glm::inverse(model));
		m_program->uniform4mat("u_inver", inver);

		m_program->uniform1i("u_shadow.map", 0);
		m_program->uniform1i("u_material.diff", m_texslot);
		m_program->uniform1i("u_material.spec", m_spcslot);
	}

	m_model->render(renderer, *m_program);
}

void WorldObject::renderWith(Renderer& renderer, Program& program) {
	program.bind();

	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_position) * m_transformation;
	program.uniform4mat("u_model", model);

	m_model->render(renderer, program);
}