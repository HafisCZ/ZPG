#pragma once

#include <glm/glm.hpp>

struct light_ptr_t;

class Light {
	private:
		unsigned int m_id;

		glm::vec3 m_position;
		glm::vec3 m_ambient;
		glm::vec3 m_diffusion;
		glm::vec3 m_specular;
		glm::vec3 m_cql;

	public:
		Light(glm::vec3 amb, glm::vec3 dif, glm::vec3 spc, glm::vec3 cql);

		inline void setPosition(glm::vec3 position) { m_position = position; }

		inline void setAmbient(glm::vec3 amb) { m_ambient = amb; }
		inline void setDiffusion(glm::vec3 dif) { m_diffusion = dif; }
		inline void setSpecular(glm::vec3 spc) { m_specular = spc; }

		inline void setConstant(float c) { m_cql.x = c; }
		inline void setLinear(float l) { m_cql.y = l; }
		inline void setQuadratic(float q) { m_cql.z = q; }

		inline glm::vec3 getColor() const { return m_ambient; }
		inline float getLinear() const { return m_cql.y; }
		inline float getQuadratic() const { return m_cql.z; }

		inline glm::vec3 getPosition() const { return m_position; }

		light_ptr_t pack() const;
};