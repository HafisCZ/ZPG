#pragma once

#include "Model.h"

class Object {
	private:
		glm::vec3 m_position;
		glm::mat4 m_transformation;

		Program* m_program;
		Model* m_model;

		unsigned int m_tex;
		unsigned int m_spc;

		bool m_shadow;

		glm::vec3 m_color;

	public:
		Object(Model& model, Program& program, unsigned int tex, unsigned int spc);
		Object(Model& model, Program& program, glm::vec3 color);
		Object(Model& model, Program& program);

		inline void setPosition(glm::vec3 position) { m_position = position; }
		inline void addPosition(glm::vec3 mod) { m_position += mod; }
		inline void setTransformation(glm::mat4 transformation) { m_transformation = transformation; }
		inline void setShadow(bool shadow) { m_shadow = shadow; }
		inline void setColor(glm::vec3 color) { m_color = color; }
		inline void setTexture(unsigned int tex) { m_tex = tex; }
		inline void setSpecular(unsigned int spc) { m_spc = spc; }
		inline void setModel(Model& model) { m_model = &model; }
		inline void setProgram(Program& program) { m_program = &program; }

		inline glm::vec3 getPosition() const { return m_position; }
		inline glm::mat4 getTransformation() const { return m_transformation; }

		void draw(Renderer& renderer);
		void draw(Renderer& renderer, Program& program);
};