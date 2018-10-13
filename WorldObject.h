#pragma once

#include "Model.h"

class WorldObject {
	private:
		unsigned int m_id;

		glm::vec3 m_position;
		glm::mat4 m_transformation;

		Program* m_program;
		Model* m_model;

		unsigned int m_texslot;
		unsigned int m_spcslot;

	public:
		WorldObject(Model& model, Program& program, unsigned int texslot = -1, unsigned int spcslot = -1);

		inline void setPosition(glm::vec3 position) { m_position = position; }
		inline void setTransformation(glm::mat4 transformation) { m_transformation = transformation; }

		inline glm::vec3 getPosition() const { return m_position; }

		void render(Renderer& renderer);
		void renderWith(Renderer& renderer, Program& program);
};