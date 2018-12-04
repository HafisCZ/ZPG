#pragma once

#include "SpatialObject.h"

#include <array>

enum LightType {
	AMBIENT, POINT, VECTOR, POINT_SHADOW
};

class Light {
	protected:
		LightType _type;
		std::array<glm::vec3, 4> _data;

	public:
		Light(LightType type) : _type(type) {}

		inline LightType getType() { return _type; }
		inline std::array<glm::vec3, 4>& getRaw() { return _data; }

		void _color(glm::vec3 color) { _data[0] = color; }
		void _vector(glm::vec3 vector) { _data[1] = vector; }
		void _intensity(float a, float d, float s) { _data[2] = { a, d, s }; }
		void _attenuation(float c, float l, float q) { _data[3] = { c, l, q }; }

		inline glm::vec3& _color() { return _data[0]; }
		inline glm::vec3& _vector() { return _data[1]; }
		inline glm::vec3& _intensity() { return _data[2]; }
		inline glm::vec3& _attenuation() { return _data[3]; }
};

class AmbientLight : public Light {
	public:
		AmbientLight(glm::vec3 color, float ai) : Light(AMBIENT) {
			_color(color);
			_intensity(ai, 0.0f, 0.0f);
		}
};

class VectorLight : public Light {
	public: 
		VectorLight(glm::vec3 color, glm::vec3 direction, float di, float si) : Light(VECTOR) {
			_color(color);
			_vector(direction);
			_intensity(0.0f, di, si);
		}
};

class PointLight : public Light {
	public:
		PointLight(glm::vec3 color, glm::vec3 position, float di, float si, float la, float qa, bool shadow = false) : Light(shadow ? POINT_SHADOW : POINT) {
			_color(color);
			_vector(position);
			_intensity(0.0f, di, si);
			_attenuation(1.0f, la, qa);
		}
};