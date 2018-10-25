#include "Light.h"

Light::Light(glm::vec3 color, glm::vec3 ambient, glm::vec3 diffusion, glm::vec3 specular, float linear, float quadratic) 
	: _color(color), _ambientIntensity(ambient), _diffusionIntensity(diffusion), _specularIntensity(specular), _linearAttenuation(linear), _quadraticAttenuation(quadratic), _position(0.0f) 
{ }