#pragma once

#include <glm/glm.hpp>

class Light {
	private:
		unsigned int m_id;

		glm::vec3 m_position;
		glm::vec3 m_ambient;
		glm::vec3 m_diffusion;
		glm::vec3 m_specular;
		glm::vec2 m_ql;

	public:
		Light(glm::vec3 amb, glm::vec3 dif, glm::vec3 spc, glm::vec2 ql);

		inline void setPosition(glm::vec3 position) { m_position = position; }

		inline void setAmbient(glm::vec3 amb) { m_ambient = amb; }
		inline void setDiffusion(glm::vec3 dif) { m_diffusion = dif; }
		inline void setSpecular(glm::vec3 spc) { m_specular = spc; }

		inline void setLinear(float l) { m_ql.x = l; }
		inline void setQuadratic(float q) { m_ql.y = q; }

		inline float getLinear() const { return m_ql.x; }
		inline float getQuadratic() const { return m_ql.y; }

		inline glm::vec3 getPosition() const { return m_position; }

		inline glm::vec3 getAmbient() { return m_ambient; }
		inline glm::vec3 getDiffusion() { return m_diffusion; }
		inline glm::vec3 getSpecular() { return m_specular; }
};