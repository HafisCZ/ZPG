#include "Object.h"

#include <glm/gtc/matrix_transform.hpp>

Object::Object(Model& model, Program& program, unsigned int tex, unsigned int spc)
	: m_model(&model), m_program(&program), m_tex(tex), m_spc(spc), m_position(1.0f), m_transformation(1.0f), m_shadow(true)
{

}

Object::Object(Model& model, Program& program, glm::vec3 color)
	: m_model(&model), m_program(&program), m_color(color), m_shadow(true), m_position(1.0f), m_transformation(1.0f), m_tex(0), m_spc(0)
{

}

Object::Object(Model& model, Program& program) : Object(model, program, glm::vec3(1.0f)) {}

void Object::draw(Renderer& renderer) {
	m_program->bind();

	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_position) * m_transformation;
	glm::mat4 inver = glm::transpose(glm::inverse(model));

	m_program->setUniform("u_model", model);
	m_program->setUniform("u_inver", inver);

	m_program->setUniform("u_shadow.map", 0);

	m_program->setUniform("u_usetex", m_spc && m_tex);
	if (m_tex && m_spc) {
		m_program->setUniform("u_material.diff", m_tex);
		m_program->setUniform("u_material.spec", m_spc);
	} else {
		m_program->setUniform("u_notex_color", m_color);
	}

	m_model->draw(renderer, *m_program);
}

void Object::draw(Renderer& renderer, Program& program) {
	if (m_shadow) {
		program.bind();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_position) * m_transformation;
		program.setUniform("u_model", model);

		m_model->draw(renderer, program);
	}
}