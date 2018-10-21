#include "Light.h"

#include "gtype.h"

Light::Light(glm::vec3 amb, glm::vec3 dif, glm::vec3 spc, glm::vec3 cql) : m_ambient(amb), m_diffusion(dif), m_specular(spc), m_cql(cql) { }

light_ptr_t Light::pack() const {
	return {
		gvec3_t(m_position),
		gvec3_t(m_ambient),
		gvec3_t(m_diffusion),
		gvec3_t(m_specular),
		gvec3_t(m_cql)
	};
}