#pragma once

#include "Model.h"

class Object {
	private:
		unsigned int m_id;

		glm::vec3 m_position;
		glm::mat4 m_transformation;

		Program* m_program;
		Model* m_model;

		unsigned int m_texslot;
		unsigned int m_spcslot;

		bool m_shadow;

	public:
		Object(Model& model, Program& program, unsigned int texslot = -1, unsigned int spcslot = -1);

		inline void setPosition(glm::vec3 position) { m_position = position; }
		inline void setTransformation(glm::mat4 transformation) { m_transformation = transformation; }
		inline void setShadow(bool shadow) { m_shadow = shadow; }

		inline glm::vec3 getPosition() const { return m_position; }

		void draw(Renderer& renderer);
		void drawWith(Renderer& renderer, Program& program);
};