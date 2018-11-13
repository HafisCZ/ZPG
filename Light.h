#pragma once

#include "SpatialObject.h"

class Light {
	private:
		glm::vec3 _color;

	public:
		Light(glm::vec3 color) : _color(color) { }
		virtual ~Light() {}

		inline glm::vec3 getColor() { return _color; }

		void setColor(glm::vec3 color) { _color = color; }
};

class AmbientLight : public Light {
	private:
		float _ai;

	public:
		AmbientLight(glm::vec3 color, float intensity) : Light(color), _ai(intensity) {}

		inline float getAmbientIntensity() { return _ai; }

		void setAmbientIntensity(float intensity) { _ai = intensity; }
};

class DirectionalLight : public Light {
	private:
		glm::vec3 _direction;
		float _di;
		float _si;

	public:
		DirectionalLight(glm::vec3 color, glm::vec3 direction, float diff, float spec) : Light(color), _direction(direction), _di(diff), _si(spec) {}

		inline glm::vec3 getDirection() { return _direction; }
		inline float getDiffuseIntensity() { return _di; }
		inline float getSpecularIntensity() { return _si; }

		void setDirection(glm::vec3 direction) { _direction = direction; }
		void setDiffuseIntensity(float diff) { _di = diff; }
		void setSpecularIntensity(float spec) { _si = spec; }
};

class PointLight : public Light, public SpatialObject {
	private:
		float _di;
		float _si;
		float _lin;
		float _qad;

	public:
		PointLight(glm::vec3 color, glm::vec3 position, float diff, float spec, float lin, float qad) : Light(color), SpatialObject(position), _di(diff), _si(spec), _lin(lin), _qad(qad) {}

		inline float getDiffuseIntensity() { return _di; }
		inline float getSpecularIntensity() { return _si; }
		inline float getLinearAttenuation() { return _lin; }
		inline float getQuadraticAttenuation() { return _qad; }

		void setDiffuseIntensity(float diff) { _di = diff; }
		void setSpecularIntensity(float spec) { _si = spec; }
		void setLinearAttenuation(float lin) { _lin = lin; }
		void setQuadraticAttenuation(float qad) { _qad = qad; }
};