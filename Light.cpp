#include "Light.h"

Light::Light(glm::vec3 amb, glm::vec3 dif, glm::vec3 spc, glm::vec2 ql) : m_ambient(amb), m_diffusion(dif), m_specular(spc), m_ql(ql), m_position(0.0f) { }