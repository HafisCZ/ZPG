#pragma once

#include "SpatialObject.h"

#include <array>

enum LightType {
	POINT, DIRECTIONAL, POINT_WITH_SHADOW, DIRECTIONAL_WIDTH_SHADOW
};

class Light {
	protected:
		glm::vec3 _color;
		LightType _type;
		std::array<glm::vec3, 5> _data;

	public:
		Light(glm::vec3 color) : _color(color) { _data[0] = color; }
		virtual ~Light() {}

		inline glm::vec3 getColor() { return _color; }

		void setColor(glm::vec3 color) { _color = _data[0] = color; }
		inline LightType getType() { return _type; }
		inline std::array<glm::vec3, 5>& getRaw() { return _data; }
};

class AmbientLight : public Light {
	private:
		float _ai;

	public:
		AmbientLight(glm::vec3 color, float intensity) : Light(color), _ai(intensity) { _data[2].x = intensity; }

		inline float getAmbientIntensity() { return _ai; }

		void setAmbientIntensity(float intensity) { _ai = _data[2].x = intensity; }
};

class DirectionalLight : public Light {
	private:
		glm::vec3 _direction;
		float _di;
		float _si;

	public:
		DirectionalLight(glm::vec3 color, glm::vec3 direction, float diff, float spec) : Light(color), _direction(direction), _di(diff), _si(spec) {
			_type = DIRECTIONAL;
			_data[1] = direction;
			_data[2].y = diff;
			_data[2].z = spec;		
		}

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
	PointLight() : Light(glm::vec3(0.0f)), SpatialObject(0.0f, 0.0f, 0.0f) {}
		PointLight(glm::vec3 color, glm::vec3 position, float diff, float spec, float lin, float qad) : Light(color), SpatialObject(position), _di(diff), _si(spec), _lin(lin), _qad(qad) {
			_type = POINT;
			_data[1] = position;
			_data[2].y = diff;
			_data[2].z = spec;
			_data[3].x = 1.0f;
			_data[3].y = lin;
			_data[3].z = qad;
		
		}

		void set(LightType t) { _type = t; }

		inline float getDiffuseIntensity() { return _di; }
		inline float getSpecularIntensity() { return _si; }
		inline float getLinearAttenuation() { return _lin; }
		inline float getQuadraticAttenuation() { return _qad; }

		void setDiffuseIntensity(float diff) { _di = diff; }
		void setSpecularIntensity(float spec) { _si = spec; }
		void setLinearAttenuation(float lin) { _lin = lin; }
		void setQuadraticAttenuation(float qad) { _qad = qad; }
};