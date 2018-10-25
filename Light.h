#pragma once

#include <glm/glm.hpp>

class Light {
	private:
		glm::vec3 _position;

		glm::vec3 _color;
		
		glm::vec3 _ambientIntensity;
		glm::vec3 _diffusionIntensity;
		glm::vec3 _specularIntensity;

		float _linearAttenuation;
		float _quadraticAttenuation;

	public:
		Light(glm::vec3 color, glm::vec3 ambient, glm::vec3 diffusion, glm::vec3 specular, float linear, float quadratic);

		void setPosition(glm::vec3 position) { _position = position; }
		void setColor(glm::vec3 color) { _color = color; }
		void setAmbientIntensity(glm::vec3 ambient) { _ambientIntensity = ambient; }
		void setDiffusionIntensity(glm::vec3 diffusion) { _diffusionIntensity = diffusion; }
		void setSpecularIntensity(glm::vec3 specular) { _specularIntensity = specular; }
		void setLinearAttenuation(float linear) { _linearAttenuation = linear; }
		void setQuadraticAttenuation(float quadratic) {	_quadraticAttenuation = quadratic; }

		inline glm::vec3 getPosition() { return _position; }
		inline glm::vec3 getColor() { return _color; }
		inline glm::vec3 getAmbientIntensity() { return _ambientIntensity; }
		inline glm::vec3 getDiffusionIntensity() { return _diffusionIntensity; }
		inline glm::vec3 getSpecularIntensity() { return _specularIntensity; }
		inline float getLinearAttenuation() { return _linearAttenuation; }
		inline float getQuadraticAttenuation() { return _quadraticAttenuation; }
};