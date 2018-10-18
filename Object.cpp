#include "Object.h"

#include <glm/gtc/matrix_transform.hpp>

Object::Object(Model& model, Program& program) : m_model(&model), m_program(&program), m_position(0.0f), m_transformation(1.0f), m_shadow(true) {}

void Object::draw(Renderer& renderer) {
	m_program->bind();

	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_position) * m_transformation;
	glm::mat4 inver = glm::transpose(glm::inverse(model));

	m_program->setUniform("u_model", model);
	m_program->setUniform("u_inver", inver);
	m_program->setUniform("texture_shadow", 0);

	m_model->draw(renderer, *m_program);
}

void Object::draw(Renderer& renderer, Program& program) {
	if (m_shadow) {
		program.bind();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_position) * m_transformation;
		program.setUniform("u_model", model);

		m_model->draw(renderer, program, true);
	}
}