#pragma once

#include <glm/vec3.hpp>

class SpatialObject {
	protected:
		glm::vec3 _position;

		bool _requiresRecalc;

	public:
		SpatialObject();
		SpatialObject(float a, float b, float c);
		SpatialObject(glm::vec3 position);

		inline glm::vec3 getPosition() { return _position; }
		inline float getX() { return _position.x; }
		inline float getY() { return _position.y; }
		inline float getZ() { return _position.z; }
		inline bool hasChanged() { return _requiresRecalc; }

		void setPosition(glm::vec3 position);
		void setPosition(float x, float y, float z);
};