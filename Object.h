#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "Program.h"
#include "Model.h"

class Object {
	private:
		glm::vec3 m_position;
		glm::mat4 m_model_matrix;

		Program* m_program;
		Model* m_model;

	public:
		Object(Model& model, Program& program) : m_model(&model), m_program(&program), m_position(0.0f), m_model_matrix(1.0f) {}

		void setPosition(glm::vec3 position) { m_position = position; }
		void setModelTransformation(glm::mat4 transformation) { m_model_matrix = transformation; }

		inline const glm::vec3 getPosition() const { return m_position; }
		inline const glm::mat4 getModelTransformation() const { return m_model_matrix; }

		inline const glm::mat4 getModelMatrix() const { return glm::translate(m_model_matrix, m_position); }

		inline const Model* getModel() const { return m_model; }
		inline const Program* getProgram() const { return m_program; }
};