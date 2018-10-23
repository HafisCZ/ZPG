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

		inline glm::vec3 getPosition() { return m_position; }
		inline glm::mat4 getModelTransformation() { return m_model_matrix; }
		
		inline glm::mat4 getModelMatrix() { return glm::translate(m_model_matrix, m_position); }
		inline glm::vec3 getBaseColor() { return glm::vec3(0.8f); }

		inline Model* getModel() { return m_model; }
		inline Program* getProgram() { return m_program; }
};